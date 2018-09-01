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
using namespace face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;

class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.model\\opencv"},
		{"in_dir", "E:\\testdata\\test-head"},
		{"out_dir", "E:\\testdata\\test-head-out-detect"},
		{"force_output", true},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"}
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
			cv::imwrite(out_file.toString(), out);

			auto msg = fmt::format("detected {:04d} faces in file {}", faces.size(), Poco::Path(in_dir).append(in_file).toString());
			logger().information(msg);
			std::cout << msg << endl;
		}

		//logger().information("para.answer.evertything=" + config().getRawString("para.list[1]"));
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
