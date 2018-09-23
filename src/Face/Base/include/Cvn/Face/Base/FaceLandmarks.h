//
// Created by tony on 18-9-23.
//

#pragma once

#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>

namespace Cvn {
    namespace Face {
        namespace Base {
            class FaceLandmarks {
                typedef std::vector<cv::Point> FPoints;

                static const FaceLandmarks & instance();

                const FPoints & ref_96_112_5pts() const;

                const FPoints & ref_112_112_5pts() const;

            private:
                FPoints _ref_96_112_5pts;
                FPoints _ref_112_112_5pts;

            public:
                FaceLandmarks();
            };

            inline const FaceLandmarks::FPoints& FaceLandmarks::ref_96_112_5pts() const {
                return _ref_96_112_5pts;
            }

            inline const FaceLandmarks::FPoints & FaceLandmarks::ref_112_112_5pts() const {
                return _ref_112_112_5pts;
            }
        }
    }
}