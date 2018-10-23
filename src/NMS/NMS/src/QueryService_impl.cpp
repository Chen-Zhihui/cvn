
#include "QueryService_impl.hpp"

std::unique_ptr<QueryServiceIf> createQueryService(
    std::string DistType,
    bool debugPrint,
    const std::string &SpaceType,
    const AnyParams &SpaceParams,
    const std::string &DataFile,
    unsigned MaxNumData,
    const std::string &MethodName,
    const std::string &LoadIndexLoc,
    const std::string &SaveIndexLoc,
    const AnyParams &IndexParams,
    const AnyParams &QueryTimeParams)
{
    ToLower(DistType);
    unique_ptr<QueryServiceIf> queryHandler;
    if (DIST_TYPE_INT == DistType)
    {
        queryHandler.reset(new QueryServiceHandler<int>(debugPrint,
                                                        SpaceType,
                                                        SpaceParams,
                                                        DataFile,
                                                        MaxNumData,
                                                        MethodName,
                                                        LoadIndexLoc,
                                                        SaveIndexLoc,
                                                        IndexParams,
                                                        QueryTimeParams));
    }
    else if (DIST_TYPE_FLOAT == DistType)
    {
        queryHandler.reset(new QueryServiceHandler<float>(debugPrint,
                                                          SpaceType,
                                                          SpaceParams,
                                                          DataFile,
                                                          MaxNumData,
                                                          MethodName,
                                                          LoadIndexLoc,
                                                          SaveIndexLoc,
                                                          IndexParams,
                                                          QueryTimeParams));
    }
    else if (DIST_TYPE_DOUBLE == DistType)
    {
        queryHandler.reset(new QueryServiceHandler<double>(debugPrint,
                                                           SpaceType,
                                                           SpaceParams,
                                                           DataFile,
                                                           MaxNumData,
                                                           MethodName,
                                                           LoadIndexLoc,
                                                           SaveIndexLoc,
                                                           IndexParams,
                                                           QueryTimeParams));
    }
    else
    {
        LOG(LIB_FATAL) << "Unknown distance value type: " << DistType;
    }

    return queryHandler;
}