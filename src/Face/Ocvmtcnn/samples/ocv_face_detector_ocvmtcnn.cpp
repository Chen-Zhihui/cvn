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
#include <Cvn/Face/Ocvmtcnn/detector.h>

using namespace cv;
using namespace face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;

using rectPoints = std::pair<cv::Rect, std::vector<cv::Point>>;

static cv::Mat drawRectsAndPoints(const cv::Mat &img,
                                  const std::vector<rectPoints> data) {
  cv::Mat outImg;
  img.convertTo(outImg, CV_8UC3);

  for (auto &d : data) {
    cv::rectangle(outImg, d.first, cv::Scalar(0, 0, 255));
    auto pts = d.second;
    for (size_t i = 0; i < pts.size(); ++i) {
      cv::circle(outImg, pts[i], 3, cv::Scalar(0, 0, 255));
    }
  }
  return outImg;
}

class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.model\\opencv"},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		{"ocvmtcnn_dir", "E:\\ws.model\\opencv-mtcnn"},
		{"in_dir", "E:\\testdata\\test-head"},
		{"out_dir", "E:\\testdata\\test-head-out-detect-ocvmtcnn"},
		{"force_output", true}
		};
		ostr << j2.dump() << std::endl;
	}

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

		Poco::Path lmtcnn_model_path(configFile().getString("ocvmtcnn_dir"));
		lmtcnn_model_path.makeDirectory();
		std::string modelDir = lmtcnn_model_path.toString();
		ProposalNetwork::Config pConfig;
  		pConfig.caffeModel = modelDir + "/det1.caffemodel";
  		pConfig.protoText = modelDir + "/det1.prototxt";
  		pConfig.threshold = 0.6f;

  		RefineNetwork::Config rConfig;
  		rConfig.caffeModel = modelDir + "/det2.caffemodel";
  		rConfig.protoText = modelDir + "/det2.prototxt";
  		rConfig.threshold = 0.7f;

  		OutputNetwork::Config oConfig;
  		oConfig.caffeModel = modelDir + "/det3.caffemodel";
  		oConfig.protoText = modelDir + "/det3.prototxt";
  		oConfig.threshold = 0.7f;

  		MTCNNDetector detector(pConfig, rConfig, oConfig);


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

			//std::vector<cv::Rect> faces;
			cv::Mat input = cv::imread(Poco::Path(in_dir).append(in_file).toString());
			Poco::Path out_file(out_dir);
			out_file.append(in_file);
			out_file.makeFile();
					   
			auto faces = detector.detect(input, 20.f, 0.709f);

			
			std::cout << "Number of faces found in the supplied image - " << faces.size()
						<< std::endl;

			std::vector<rectPoints> data;

			  // show the image with faces in it
			for (size_t i = 0; i < faces.size(); ++i) {
				std::vector<cv::Point> pts;
				for (int p = 0; p < NUM_PTS; ++p) {
				  pts.push_back(
					  cv::Point(faces[i].ptsCoords[2 * p], faces[i].ptsCoords[2 * p + 1]));
				}

				auto rect = faces[i].bbox.getRect();
				auto d = std::make_pair(rect, pts);
				data.push_back(d);
			}

			auto resultImg = drawRectsAndPoints(input, data);

			cv::imwrite(out_file.toString(), resultImg);

			auto msg = fmt::format("detected {:04d} faces in file {}", faces.size(), Poco::Path(in_dir).append(in_file).toString());
			logger().information(msg);
			std::cout << msg << endl;
		}

		//logger().information("para.answer.evertything=" + config().getRawString("para.list[1]"));
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
