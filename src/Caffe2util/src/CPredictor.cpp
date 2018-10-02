#include <Cvn/Caffe2util/CPredictor.h>
#include <caffe2/predictor/predictor_utils.h>
#include <caffe2/utils/proto_utils.h>
#include <Poco/Path.h>


bool CPredictor::load(const std::string & jsonstr) {
    bool ok = true;
    json js;
    try {
        js = json::parse(jsonstr);
    } catch ( ... ) {
        ok = false;
        return false;
    }

    try {
        Poco::Path dir(js["dir"].get<std::string>());
        dir.makeDirectory();

        auto init_path = dir; 
        auto init_file = init_path.append(js["init_net"].get<std::string>()).makeFile().toString(); 

        auto pred_path = dir;
        auto pred_file = pred_path.append(js["predict_net"].get<std::string>()).makeFile().toString();

        bool loadok = true;
        loadok = caffe2::ReadProtoFromBinaryFile(pred_file, &_prednet);
        if(!loadok) {
            std::cout <<"load prednet failed with filename = "<<pred_file<<std::endl;
            return false;
        }

        loadok = caffe2::ReadProtoFromBinaryFile(init_file, &_initnet);
        if( !loadok ) {
            std::cout <<"load initnet failed with filename = "<<init_file<<std::endl;
            return false;
        }

        auto config = caffe2::makePredictorConfig(_initnet, _prednet);
        config.input_names = js["input_names"].get<std::vector<std::string>>();
        config.output_names= js["output_names"].get<std::vector<std::string>>();
        // for(auto & ele : js["input"]) {
        //     config.input_names.append(ele.get<std::string>());
        // }
        // for(auto & ele : js["output"]) {
        //     config.output_names.append(ele.get<std::string)
        // }
        _predictor = std::make_shared<caffe2::Predictor>(config);
    } catch (...) {
        return false;
    }
    return true;
}