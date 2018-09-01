
/*
read from
cam
video
imageseq
dir: png, jpg, bmp, etc
*/

#include <Cvn/Cvutil/ImageCapture.h>

namespace Cvn {
	namespace Cvutil {

		using namespace cv;

		ImageCapture::ImageCapture() {

		}

		ImageCapture::~ImageCapture() {

		}

		bool ImageCapture::open(const cv::String& filename) {
			//TODO: if filename is a DIR, open dir
			return cap.open(filename);
		}

		bool ImageCapture::open(int index) {
			return cap.open(index);
		}
		bool ImageCapture::open(int cameraNum, int apiPreference)
		{
			return cap.open(cameraNum, apiPreference);
		}
		bool ImageCapture::isOpened() const
		{
			return cap.isOpened();
		}
		void ImageCapture::release()
		{
			//todo: close dir
			cap.release();
		}
		bool ImageCapture::grab()
		{
			return cap.grab();
		}
		bool ImageCapture::retrieve(OutputArray image, int flag)
		{
			return cap.retrieve(image, flag);
		}
		ImageCapture& ImageCapture::operator >> (Mat& image) {
			cap >> image;
			return *this;
		}
		ImageCapture& ImageCapture::operator >> (UMat& image) {
			cap >> image;
			return *this;
		}
		bool ImageCapture::read(OutputArray image)
		{
			return cap.read(image);
		}
		bool ImageCapture::set(int propId, double value)
		{
			return cap.set(propId, value);
		}
		double ImageCapture::get(int propId) const
		{
			return cap.get(propId);
		}
		bool ImageCapture::open(const String & filename, int apiPreference)
		{
			return cap.open(filename,apiPreference);
		}
	}
}