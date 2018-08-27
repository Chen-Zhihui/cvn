// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Cvn/Apputil/TmplApp.h>
#include <Cvn/Face/Base/Face.h>
#include <nlohmann/json.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/face.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <memory>

using namespace cv;
using namespace face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;

class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 = {
		  {"opencv_dir", "E:\\ws.cvn\\cvn.model\\opencv"},
		  {"infile", "E:\\testdata\\test-head"},
		  {"outfile_base", "E:\\testdata\\test-head-out"},
		  {"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		  {"test_dir" , "E:\\testdata"},
		  {"test_dir_out", "E:\\testdata\\test-out"}
		};
		ostr << j2.dump() << std::endl;
	}

	std::shared_ptr<cv::face::CParams> cparams;

	cv::CascadeClassifier face_cascade;

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

		Poco::Path face_cascade_model_path(configFile().getString("opencv_dir"));
		face_cascade_model_path.append(configFile().getString("detector_model"));

		cparams.reset(new cv::face::CParams(face_cascade_model_path.toString()));

		std::vector<cv::Rect> faces;

		cv::Mat input = cv::imread(configFile().getString("infile"));
		cv::face::getFaces(input, faces, cparams.get());
		
		auto out = input.clone();
		cvn::Face::Base::drawFacemarks(out, faces, Scalar(0, 255, 0));
		//cv::face::drawFacemarks();

		cv::imwrite(configFile().getString("outfile"), out);

		//logger().information("para.answer.evertything=" + config().getRawString("para.list[1]"));
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
