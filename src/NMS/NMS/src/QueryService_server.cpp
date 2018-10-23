#include <memory>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

#include "QueryService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>

#include <boost/program_options.hpp>

#include "params.h"
#include "params_def.h"
#include "utils.h"
#include "space.h"
#include "spacefactory.h"
#include "index.h"
#include "rangequery.h"
#include "knnquery.h"
#include "knnqueue.h"
#include "methodfactory.h"
#include "init.h"
#include "logging.h"
#include "ztimer.h"

#define MAX_SPIN_LOCK_QTY 1000000
#define SLEEP_DURATION    10


const unsigned THREAD_COEFF = 4;

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using std::string;
using std::unique_ptr;
using std::exception;
using std::mutex;
using std::unique_lock;

using namespace  ::similarity;

#include "QueryService_impl.hpp"


namespace po = boost::program_options;

static void Usage(const char *prog,
                  const po::options_description& desc) {
    std::cout << prog << std::endl
              << desc << std::endl;
}

void ParseCommandLineForServer(int argc, char*argv[],
                      bool&                   debugPrint,
                      string&                 LoadIndexLoc,
                      string&                 SaveIndexLoc,
                      int&                    port,
                      size_t&                 threadQty,
                      string&                 LogFile,
                      string&                 DistType,
                      string&                 SpaceType,
                      std::shared_ptr<AnyParams>&  SpaceParams,
                      string&                 DataFile,
                      unsigned&               MaxNumData,
                      string&                         MethodName,
                      std::shared_ptr<AnyParams>&     IndexTimeParams,
                      std::shared_ptr<AnyParams>&     QueryTimeParams) {
  string          methParams;
  size_t          defaultThreadQty = THREAD_COEFF * thread::hardware_concurrency();

  string          indexTimeParamStr;
  string          queryTimeParamStr;
  string          spaceParamStr;

  po::options_description ProgOptDesc("Allowed options");
  ProgOptDesc.add_options()
    (HELP_PARAM_OPT.c_str(),          HELP_PARAM_MSG.c_str())
    (DEBUG_PARAM_OPT.c_str(),         po::bool_switch(&debugPrint), DEBUG_PARAM_MSG.c_str())
    (PORT_PARAM_OPT.c_str(),          po::value<int>(&port)->required(), PORT_PARAM_MSG.c_str())
    (THREAD_PARAM_OPT.c_str(),        po::value<size_t>(&threadQty)->default_value(defaultThreadQty), THREAD_PARAM_MSG.c_str())
    (LOG_FILE_PARAM_OPT.c_str(),      po::value<string>(&LogFile)->default_value(LOG_FILE_PARAM_DEFAULT), LOG_FILE_PARAM_MSG.c_str())
    (SPACE_TYPE_PARAM_OPT.c_str(),    po::value<string>(&spaceParamStr)->required(),                SPACE_TYPE_PARAM_MSG.c_str())
    (DIST_TYPE_PARAM_OPT.c_str(),     po::value<string>(&DistType)->default_value(DIST_TYPE_FLOAT), DIST_TYPE_PARAM_MSG.c_str())
    (DATA_FILE_PARAM_OPT.c_str(),     po::value<string>(&DataFile)->required(),                     DATA_FILE_PARAM_MSG.c_str())
    (MAX_NUM_DATA_PARAM_OPT.c_str(),  po::value<unsigned>(&MaxNumData)->default_value(MAX_NUM_DATA_PARAM_DEFAULT), MAX_NUM_DATA_PARAM_MSG.c_str())
    (METHOD_PARAM_OPT.c_str(),        po::value<string>(&MethodName)->default_value(METHOD_PARAM_DEFAULT), METHOD_PARAM_MSG.c_str())
    (LOAD_INDEX_PARAM_OPT.c_str(),    po::value<string>(&LoadIndexLoc)->default_value(LOAD_INDEX_PARAM_DEFAULT),   LOAD_INDEX_PARAM_MSG.c_str())
    (SAVE_INDEX_PARAM_OPT.c_str(),    po::value<string>(&SaveIndexLoc)->default_value(SAVE_INDEX_PARAM_DEFAULT),   SAVE_INDEX_PARAM_MSG.c_str())
    (QUERY_TIME_PARAMS_PARAM_OPT.c_str(), po::value<string>(&queryTimeParamStr)->default_value(""), QUERY_TIME_PARAMS_PARAM_MSG.c_str())
    (INDEX_TIME_PARAMS_PARAM_OPT.c_str(), po::value<string>(&indexTimeParamStr)->default_value(""), INDEX_TIME_PARAMS_PARAM_MSG.c_str())
    ;

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, ProgOptDesc), vm);
    po::notify(vm);
  } catch (const exception& e) {
    Usage(argv[0], ProgOptDesc);
    LOG(LIB_FATAL) << e.what();
  }

  if (vm.count("help")  ) {
    Usage(argv[0], ProgOptDesc);
    exit(0);
  }

  if (vm.count("method") != 1) {
    Usage(argv[0], ProgOptDesc);
    LOG(LIB_FATAL) << "There should be exactly one method specified!";
  }

  ToLower(DistType);
  ToLower(spaceParamStr);
  ToLower(MethodName);
 
  try {
    {
      vector<string>     desc;
      ParseSpaceArg(spaceParamStr, SpaceType, desc);
      SpaceParams = shared_ptr<AnyParams>(new AnyParams(desc));
    }

    {
      vector<string>     desc;
      ParseArg(indexTimeParamStr, desc);
      IndexTimeParams = shared_ptr<AnyParams>(new AnyParams(desc));
    }

    {
      vector<string>  desc;

      ParseArg(queryTimeParamStr, desc);
      QueryTimeParams = shared_ptr<AnyParams>(new AnyParams(desc));
    }
    
    if (DataFile.empty()) {
      LOG(LIB_FATAL) << "data file is not specified!";
    }

    if (!DoesFileExist(DataFile)) {
      LOG(LIB_FATAL) << "data file " << DataFile << " doesn't exist";
    }
  } catch (const exception& e) {
    LOG(LIB_FATAL) << "Exception: " << e.what();
  }
}

int main(int argc, char *argv[]) {
  bool        debugPrint = 0;
  int         port = 0;
  size_t      threadQty = 0;
  string      LogFile;
  string      DistType;
  string      SpaceType;
  std::shared_ptr<AnyParams>  SpaceParams;
  string      DataFile;
  unsigned    MaxNumData;
  
  string                         MethodName;
  std::shared_ptr<AnyParams>     IndexParams;
  std::shared_ptr<AnyParams>     QueryTimeParams;

  string      LoadIndexLoc;
  string      SaveIndexLoc;

  ParseCommandLineForServer(argc, argv,
                      debugPrint,
                      LoadIndexLoc,
                      SaveIndexLoc,
                      port,
                      threadQty,
                      LogFile,
                      DistType,
                      SpaceType,
                      SpaceParams,
                      DataFile,
                      MaxNumData,
                      MethodName,
                      IndexParams,
                      QueryTimeParams
  );

  initLibrary(0, LogFile.empty() ? LIB_LOGSTDERR:LIB_LOGFILE, LogFile.c_str());

  ToLower(DistType);

  unique_ptr<QueryServiceIf>   queryHandler;

  if (DIST_TYPE_INT == DistType) {
    queryHandler.reset(new QueryServiceHandler<int>(debugPrint,
                                                    SpaceType,
                                                    *SpaceParams,
                                                    DataFile,
                                                    MaxNumData,
                                                    MethodName,
                                                    LoadIndexLoc,
                                                    SaveIndexLoc,
                                                    *IndexParams,
                                                    *QueryTimeParams));
  } else if (DIST_TYPE_FLOAT == DistType) {
    queryHandler.reset(new QueryServiceHandler<float>(debugPrint,
                                                    SpaceType,
                                                    *SpaceParams,
                                                    DataFile,
                                                    MaxNumData,
                                                    MethodName,
                                                    LoadIndexLoc,
                                                    SaveIndexLoc,
                                                    *IndexParams,
                                                    *QueryTimeParams));
  } else if (DIST_TYPE_DOUBLE == DistType) {
    queryHandler.reset(new QueryServiceHandler<double>(debugPrint,
                                                    SpaceType,
                                                    *SpaceParams,
                                                    DataFile,
                                                    MaxNumData,
                                                    MethodName,
                                                    LoadIndexLoc,
                                                    SaveIndexLoc,
                                                    *IndexParams,
                                                    *QueryTimeParams));
  
  } else {
    LOG(LIB_FATAL) << "Unknown distance value type: " << DistType;
  }

  boost::shared_ptr<QueryServiceIf> handler(queryHandler.get());
  boost::shared_ptr<TProcessor> processor(new QueryServiceProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

#if SIMPLE_SERVER
  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  LOG(LIB_INFO) << "Started a simple server.";
#else
  boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(threadQty);
  boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  TThreadPoolServer server(processor,
                           serverTransport,
                           transportFactory,
                           protocolFactory,
                           threadManager);
  LOG(LIB_INFO) << "Started a server with a " << threadQty << " thread-pool.";
#endif
  server.serve();
  return 0;
}