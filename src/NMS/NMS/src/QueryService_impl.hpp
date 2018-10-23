/**
 * Non-metric Space Library
 *
 * Main developers: Bilegsaikhan Naidan, Leonid Boytsov, Yury Malkov, Ben Frederickson, David Novak
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib
 *
 * Copyright (c) 2013-2018
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */
#include <memory>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

#include "QueryService.h"

#include <nmslib/params.h>
#include <nmslib/params_def.h>
#include <nmslib/utils.h>
#include <nmslib/space.h>
#include <nmslib/spacefactory.h>
#include <nmslib/index.h>
#include <nmslib/rangequery.h>
#include <nmslib/knnquery.h>
#include <nmslib/knnqueue.h>
#include <nmslib/methodfactory.h>
#include <nmslib/init.h>
#include <nmslib/logging.h>
#include <nmslib/ztimer.h>

#define MAX_SPIN_LOCK_QTY 1000000
#define SLEEP_DURATION    10


const unsigned THREAD_COEFF = 4;


using std::string;
using std::unique_ptr;
using std::exception;
using std::mutex;
using std::unique_lock;

using namespace  ::similarity;

class LockedCounterManager {
public:
  LockedCounterManager(int& counter, mutex& mtx) : counter_(counter), mtx_(mtx) 
  {
    unique_lock<mutex> lock(mtx_);
    ++counter_;
  }

  ~LockedCounterManager() {
    unique_lock<mutex> lock(mtx_);
    --counter_;
  }

private:
  int&     counter_; 
  mutex&   mtx_;
};

template <class dist_t>
class QueryServiceHandler : virtual public QueryServiceIf {
 public:
  QueryServiceHandler(
                      bool                               debugPrint,
                      const string&                      SpaceType,
                      const AnyParams&                   SpaceParams,
                      const string&                      DataFile,
                      unsigned                           MaxNumData,
                      const string&                      MethodName,
                      const string&                      LoadIndexLoc,
                      const string&                      SaveIndexLoc,
                      const AnyParams&                   IndexParams,
                      const AnyParams&                   QueryTimeParams) :
    debugPrint_(debugPrint),
    methName_(MethodName),
    space_(SpaceFactoryRegistry<dist_t>::Instance().CreateSpace(SpaceType, SpaceParams)),
    counter_(0)

  {
    unique_ptr<DataFileInputState> inpState(space_->ReadDataset(dataSet_,
                                                                externIds_,
                                                                DataFile,
                                                                MaxNumData));
    space_->UpdateParamsFromFile(*inpState);

    CHECK(dataSet_.size() == externIds_.size());

    index_.reset(MethodFactoryRegistry<dist_t>::Instance().
                                CreateMethod(true /* print progress */,
                                        methName_,
                                        SpaceType,
                                        *space_.get(),
                                        dataSet_));

    if (!LoadIndexLoc.empty() && DoesFileExist(LoadIndexLoc)) {
      LOG(LIB_INFO) << "Loading index from location: " << LoadIndexLoc; 
      index_->LoadIndex(LoadIndexLoc);
      LOG(LIB_INFO) << "The index is loaded!";
    } else {
      LOG(LIB_INFO) << "Creating a new index copy"; 
      index_->CreateIndex(IndexParams);      
      LOG(LIB_INFO) << "The index is created!";
    }

    if (!SaveIndexLoc.empty() && !DoesFileExist(SaveIndexLoc)) {
      LOG(LIB_INFO) << "Saving the index";
      index_->SaveIndex(SaveIndexLoc);
      LOG(LIB_INFO) << "The index is saved!";
    }

    LOG(LIB_INFO) << "Setting query-time parameters";
    index_->SetQueryTimeParams(QueryTimeParams);
  }

  ~QueryServiceHandler() {
    for (auto e: dataSet_) delete e;
  }

  void setQueryTimeParams(const string& queryTimeParamStr) {
    try {
      // Query time parameters are essentially spin-locked
      while (true) {
        for (size_t i = 0; i < MAX_SPIN_LOCK_QTY; ++i) {
          unique_lock<mutex> lock(mtx_);
          if (0 == counter_) {
            vector<string>  desc;
            ParseArg(queryTimeParamStr, desc);
            if (debugPrint_) {
              LOG(LIB_INFO) << "Setting query time parameters (" << queryTimeParamStr << ")";
              for (string s: desc) {
                LOG(LIB_INFO) << s;
              }
            }
            index_->SetQueryTimeParams(AnyParams(desc));
            return;
          }
        } // the lock will be released in the end of the block

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
      }
    } catch (const exception& e) {
        throw e;
    } catch (...) {
        std::exception e("unknown");
        throw e;
    }
  }

  void rangeQuery(ReplyEntryList& _return, const double r, const string& queryObjStr, 
                  const bool retExternId, const bool retObj) {
    // This will increase the counter and prevent modification of query time parameters.
    LockedCounterManager  mngr(counter_, mtx_);

    try {
      if (debugPrint_) {
        LOG(LIB_INFO) << "Running a range query, r=" << r << " retExternId=" << retExternId << " retObj=" << retObj;
      }
      WallClockTimer wtm;

      wtm.reset();

      unique_ptr<Object>  queryObj(space_->CreateObjFromStr(0, -1, queryObjStr, NULL));

      RangeQuery<dist_t> range(*space_, queryObj.get(), r);
      index_->Search(&range, -1);

      _return.clear();

      wtm.split();

      if (debugPrint_) {
        LOG(LIB_INFO) << "Finished in: " << wtm.elapsed() / 1e3f << " ms";
      }

      vector<IdType> ids;
      vector<double> dists;
      vector<string> externIds;
      vector<string> objs;

     
      if (debugPrint_) { 
        LOG(LIB_INFO) << "Results: ";
      }
      
      const ObjectVector&     vResObjs  = *range.Result(); 
      const vector<dist_t>&   vResDists = *range.ResultDists();

      for (size_t i = 0; i < vResObjs.size(); ++i) {
        const Object* pObj = vResObjs[i];
        dist_t dist        = vResDists[i];

        ReplyEntry e;

        e.__set_id(pObj->id());
        e.__set_dist(dist);

        if (debugPrint_) {
          ids.insert(ids.begin(), e.id);
          dists.insert(dists.begin(), e.dist);
        }

        string externId;

        if (retExternId || retObj) {
          CHECK(e.id < externIds_.size());
          externId = externIds_[e.id];
          e.__set_externId(externId);
          externIds.insert(externIds.begin(), e.externId);
        }

        if (retObj) {
          const string& s = space_->CreateStrFromObj(pObj, externId);
          e.__set_obj(s);
          if (debugPrint_) {
            objs.insert(objs.begin(), s);
          }
        }
        _return.insert(_return.begin(), e);
      }
      if (debugPrint_) {
        for (size_t i = 0; i < ids.size(); ++i) {
          LOG(LIB_INFO) << "id=" << ids[i] << " dist=" << dists[i] << ( retExternId ? " " + externIds[i] : string(""));
          if (retObj) LOG(LIB_INFO) << objs[i]; 
        }
      }
    } catch (const exception& e) {
        throw e;
    } catch (...) {
        std::exception e("unknown");
        throw e;
    } 
  }

  double getDistance(const std::string& objStr1, const std::string& objStr2) {
    try {
      if (debugPrint_) {
        LOG(LIB_INFO) << "Computing the distance between two objects";
      }

      WallClockTimer wtm;

      wtm.reset();

      unique_ptr<Object>  obj1(space_->CreateObjFromStr(0, -1, objStr1, NULL));
      unique_ptr<Object>  obj2(space_->CreateObjFromStr(0, -1, objStr2, NULL));

      double res = space_->IndexTimeDistance(obj1.get(), obj2.get());

      wtm.split();

      if (debugPrint_) {
        LOG(LIB_INFO) << "Result: " << res;
        LOG(LIB_INFO) << "Finished in: " << wtm.elapsed() / 1e3f << " ms ";
      }
      return res;
    } catch (const exception& e) {
        throw e;
    } catch (...) {
        std::exception e("unknown");
        throw e;
    }
  }

  void knnQuery(ReplyEntryList& _return, const int32_t k, 
                const std::string& queryObjStr, const bool retExternId, const bool retObj) {
    // This will increase the counter and prevent modification of query time parameters.
    LockedCounterManager  mngr(counter_, mtx_);

    try {
      if (debugPrint_) {
        LOG(LIB_INFO) << "Running a " << k << "-NN query" << " retExternId=" << retExternId << " retObj=" << retObj;
      }
      WallClockTimer wtm;

      wtm.reset();

      unique_ptr<Object>  queryObj(space_->CreateObjFromStr(0, -1, queryObjStr, NULL));

      KNNQuery<dist_t> knn(*space_, queryObj.get(), k);
      index_->Search(&knn, -1);
      unique_ptr<KNNQueue<dist_t>> res(knn.Result()->Clone());

      _return.clear();

      wtm.split();

      if (debugPrint_) {
        LOG(LIB_INFO) << "Finished in: " << wtm.elapsed() / 1e3f << " ms";
      }

      vector<IdType> ids;
      vector<double> dists;
      vector<string> objs;
      vector<string> externIds;

     
      if (debugPrint_) { 
        LOG(LIB_INFO) << "Results: ";
      }

      while (!res->Empty()) {
        const Object* topObj = res->TopObject();
        dist_t topDist = res->TopDistance();

        ReplyEntry e;

        e.__set_id(topObj->id());
        e.__set_dist(topDist);

        if (debugPrint_) {
          ids.insert(ids.begin(), e.id);
          dists.insert(dists.begin(), e.dist);
        }

        string externId;

        if (retExternId || retObj) {
          CHECK(e.id < externIds_.size());
          externId = externIds_[e.id];
          e.__set_externId(externId);
          externIds.insert(externIds.begin(), e.externId);
        }

        if (retObj) {
          const string& s = space_->CreateStrFromObj(topObj, externId);
          e.__set_obj(s);
          if (debugPrint_) {
            objs.insert(objs.begin(), s);
          }
        }
        _return.insert(_return.begin(), e);
        res->Pop();
      }
      if (debugPrint_) {
        for (size_t i = 0; i < ids.size(); ++i) {
          LOG(LIB_INFO) << "id=" << ids[i] << " dist=" << dists[i] << ( retExternId ? " " + externIds[i] : string(""));
          if (retObj) LOG(LIB_INFO) << objs[i]; 
        }
      }
    } catch (const exception& e) {
        throw e;
    } catch (...) {
        std::exception e("unknown");
        throw e;
    }


  }

 private:
  bool                        debugPrint_;
  string                      methName_;
  unique_ptr<Space<dist_t>>   space_;
  unique_ptr<Index<dist_t>>   index_;
  vector<string>              externIds_;
  ObjectVector                dataSet_; 

  int                         counter_; 
  mutex                       mtx_;
};



