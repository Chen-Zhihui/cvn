#pragma once

#include <caffe2/core/init.h>
#include <caffe2/predictor/predictor.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;


class CPredictor {
    public:
      json config =
          {
              {"dir", "E:\\testdata\\test-head-out-detect"},
              {"name", "sphereface_20"},
              {"input", {"data"}},
              {"output", {"fc5"}}};

      virtual bool load(const std::string & jsonstr) ;

    protected: 

    caffe2::NetDef _initNet, _predictNet;
    std::shared_ptr<caffe2::Predictor> _predictor;
};