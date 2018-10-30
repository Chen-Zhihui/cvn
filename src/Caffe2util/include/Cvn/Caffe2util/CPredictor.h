#pragma once

#include <caffe2/core/logging.h>
#include <caffe2/core/init.h>
#include <caffe2/predictor/predictor.h>
#include <memory>
#include <iostream>


class CPredictor {
    public:
    //   const json sample_json =
    //       {
    //           {"dir", "/home/tony/data/ws.cv.face.model/caffe2/sphereface/"},
    //           {"predict_net", "sphereface_20_predict_net.pb"},
    //           {"init_net",      "sphereface_20_init_net.pb"},
    //           {"input_names", {"data"}},
    //           {"output_names", {"fc5"}}};

      bool load(const std::string & jsonstr) ;

      bool operator()(const caffe2::Predictor::TensorMap & input,
                      caffe2::Predictor::TensorMap * outputs);

    protected: 

    // json config_loaded;
    caffe2::NetDef _initnet, _prednet;
    std::shared_ptr<caffe2::Predictor> _predictor;
};