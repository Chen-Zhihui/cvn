
/*
通用的定义，全放在此处
*/

#pragma  once

#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>

namespace cvn
{
	namespace Face {
		namespace Base {

			inline void drawFaceRecs(cv::Mat & image, const std::vector<cv::Rect> & faces, cv::Scalar color) {
			 	for(auto & face : faces) {
			 		cv::rectangle(image, face, color, 2);
			 	}
			}

			inline void drawFaceRecs(cv::Mat & image, const std::vector<cv::Rect2d> & faces, cv::Scalar color) {
				for(auto & face : faces) {					
			 		cv::rectangle(image, (cv::Rect)(face), color, 2);
			 	}
			}

			inline void drawFaceRec(cv::Mat & image, const cv::Rect & face, cv::Scalar color) {
				cv::rectangle(image, face, color, 2);
			}

			inline void drawFacemark(cv::Mat & image, const cv::InputArray points, const cv::Scalar color) {
				std::vector<cv::Point2f> pts = points.getMat();
				for (size_t i = 0; i < pts.size(); i++) {
					cv::circle(image, pts[i], 3, color, -1);
				}
			}

		}
	}
}