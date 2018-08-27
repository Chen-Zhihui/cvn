
/*
通用的定义，全放在此处
*/

#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>

namespace cvn
{
	namespace Face {
		namespace Base {
			void drawFaceRecs(cv::InputOutputArray image, const std::vector<cv::Rect> & faces, cv::Scalar color) {
				for (int j = 0; j < faces.size(); j++) {
					cv::rectangle(image, faces[j], color, 2);
				}
			}
		}
	}
}