#pragma once

#include <opencv/cv.hpp>
#include <caffe2/predictor/predictor.h>

class CPredictor;
class SperefaceFeature {
    public:
    const int input_width = 112;
    const int input_width96 = 96;
    const int input_height= 112;
    const int nc = 3;
    const int bs = 1;

    SperefaceFeature();

    bool init(const std::string & configjs);

    std::vector<float> feature(const cv::Mat & aligned ) const ;

    protected:

    mutable caffe2::Predictor::TensorMap  input_vec;
    std::shared_ptr<CPredictor> predictor;
};