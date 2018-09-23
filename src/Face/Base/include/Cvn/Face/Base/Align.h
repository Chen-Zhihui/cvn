//
// Created by tony on 18-9-23.
//

#pragma  once

#include <opencv2/opencv.hpp>
#include <Cvn/Face/Base/FaceLandmarks.h>
#include <Cvn/Cvutil/cp2tform.h>
#include <Poco/Bugcheck.h>

namespace Cvn {
    namespace Face {
        namespace Base {
            cv::Mat align(const cv::Mat & sample, const Face::Base::FaceLandmarks::FPoints & fpts, int width)
            {
                poco_assert(width == 96 || width == 112) << "Face align only support width to be 96 or 112";
                poco_assert(fpts.size() == 5); //TODO 68
                auto & landmarks = width == 112 ?
                                   Face::Base::FaceLandmarks::get().ref_112_112_5pts() :
                        Face::Base::FaceLandmarks::get().ref_96_112_5pts();
                return align(sample, fpts, landmarks, Cvn::Cvutil::cp2tform_similarity);
            }
        }
    }
}