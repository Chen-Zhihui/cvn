// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Poco/File.h>
#include <Poco/Path.h>
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
using namespace cv::face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;



bool myDetector(InputArray image, OutputArray faces, CascadeClassifier *face_cascade)
{
	Mat gray;

	if (image.channels() > 1)
		cvtColor(image, gray, COLOR_BGR2GRAY);
	else
		gray = image.getMat().clone();

	equalizeHist(gray, gray);

	std::vector<Rect> faces_;
	face_cascade->detectMultiScale(gray, faces_, 1.4, 2, CASCADE_SCALE_IMAGE, Size(30, 30));
	Mat(faces_).copyTo(faces);
	return true;
}


class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.cvn\\cvn.model\\opencv"},
		{"in_dir", "E:\\testdata\\test-head-onlyone"},
		{"out_dir", "E:\\testdata\\test-head-out-landmark"},
		{"force_output", true},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		{"lbf_model", "E:\\ws.cvn\\GSOC2017\\data\\lbfmodel.yaml"}
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


		Poco::Path in_dir(configFile().getString("in_dir"));   in_dir = in_dir.makeDirectory();
		Poco::Path out_dir(configFile().getString("out_dir")); out_dir = out_dir.makeDirectory();

		Poco::File out(out_dir);
		if (out.exists()) {
			if (configFile().getBool("force_output") && out.isDirectory()) {
			}
			else {
				std::cerr << "out put directory already exists, failed" << std::endl;
				logger().information("out put directory already exists, failed");
				return -1;
			}
		}
		out.createDirectories();

		Poco::File in(in_dir);
		if (!in.exists() || in.isFile() ) {
			std::cerr << "input dir doesnot exists" << std::endl;
			logger().information("input dir donnot exists");
			return  -1;
		}

		std::vector<std::string> files;
		in.list(files);

		// Create an instance of Facemark
		Ptr<Facemark> facemark = FacemarkLBF::create();
		Poco::Path lbf_path(configFile().getString("lbf_model"));
		Poco::File lbf_file(lbf_path);
		if (!lbf_file.exists() || !lbf_file.isFile()) {
			auto msg = fmt::format("lbf model Not found {}", lbf_path.toString());
			logger().information(msg);
			cerr << msg << endl; 
			return -1;
		}
		// Load landmark detector
		facemark->loadModel(lbf_path.toString());

		int count = 0;
		for (auto & in_file : files) {
			Poco::Path file_path(in_dir);
			file_path.append(in_file);
			auto dir = file_path.makeDirectory();
			auto file = file_path.makeFile();
			if (!Poco::File(file).exists())
				continue;

			std::vector<cv::Rect> faces;
			cv::Mat input = cv::imread(Poco::Path(in_dir).append(in_file).toString());
			Poco::Path out_file(out_dir);
			out_file.append(in_file);
			out_file.makeFile();
					   
			cv::face::getFaces(input, faces, cparams.get());
			auto out = input.clone();
			cvn::Face::Base::drawFaceRecs(out, faces, Scalar(0, 255, 0));

			auto msg = fmt::format("detected {:04d} faces in file {}", faces.size(), Poco::Path(in_dir).append(in_file).toString());
			logger().information(msg);
			std::cout << msg << endl;

			std::vector<std::vector<Point2f> > landmarks;
			if( !faces.empty())
				auto ret = facemark->fit(input, faces, landmarks);
			if (!landmarks.empty() ) {
				drawFacemarks(out, landmarks);
			}
			else {
				auto msg = fmt::format("landmark faces failed in file {}", Poco::Path(in_dir).append(in_file).toString());
			}

			cv::imwrite(out_file.toString(), out);
			count++;
		}
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
