//
// Created by tony on 18-9-23.
//

#pragma once

#include <opencv2/opencv.hpp>

namespace Cvn {
    namespace Cvutil {

    /// @brief cp2tform matlab export to c++
    /// /// @param type: 'similarity' or 'nonreflective similarity'

    enum cp2tform_type {
        cp2tform_similarity,
        cp2tform_noreflective
    };

    typedef std::vector<cv::Point> FPoints;

    cv::Mat cp2tform(const FPoints &src, const FPoints &dst, cp2tform_type type = cp2tform_similarity);

    cv::Mat align(const cv::Mat & sample, const std::vector<cv::Point> & fpts,
            const std::vector<cv::Point> & ref_fpts, cp2tform_type type = cp2tform_similarity);

    }
}
