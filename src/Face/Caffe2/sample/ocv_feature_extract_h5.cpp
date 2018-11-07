
// cpptmpl.cpp : Defines the entry point for the application.
//

#include <caffe2/core/logging.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Util/ConfigurationView.h>
#include <Cvn/Apputil/TmplApp.h>
#include <Cvn/Face/Base/Face.h>
#include <Cvn/Face/Caffe2/extract.h>
#include <Cvn/Face/Caffe2/sphereface.h>
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

class DetectorApp : public TmplApp
{
  public:
	virtual void dumpParamList(std::ostream &ostr) const
	{
		json j2 =
		{ {"extractor" , {
			 {"input_dir", "E:\\testdata_inuse\\webfaces-imgs"},
			 {"output_dir", "E:\\ws.cvn"},
			 {"h5file", "webfaces-2000by4.h5"},
			 {"indexfile", "webfaces-2000by4.index"},
			 {"maxFolder", 1},
			 {"maxImg", 4}},
			 },
		{"facemodel" , {
				 {"dir", "E:\\ws_face.model\\caffe2\\sphereface"},
				 {"predict_net", "sphereface_20_predict_net.pb"},
				 {"init_net", "sphereface_20_init_net.pb"},
				 {"input_names", {"data"}},
				 {"output_names", {"fc5"}}
			 }}
			};
		ostr << j2.dump() << std::endl;
	}

	int main(const Poco::Util::Application::ArgVec &args)
	{
		printConfig(this->configFile());

		auto extractorConfig = configFile().createView("extractor");

		printConfig(*extractorConfig);

		Poco::Path in_dir(extractorConfig->getString("input_dir"));
		in_dir = in_dir.makeDirectory();
		Poco::Path out_dir(extractorConfig->getString("output_dir"));
		out_dir = out_dir.makeDirectory();

		auto h5File = out_dir; h5File.append(extractorConfig->getString("h5file", "h5file.h5"));
		auto indexfile = out_dir; indexfile.append(extractorConfig->getString("indexfile", "indexfile.index"));

		auto maxFolder = extractorConfig->getInt64("maxFolder", 10);
		auto maxImg = extractorConfig->getInt64("maxImg", 10);

		Poco::File out(out_dir);
		out.createDirectories();

		Poco::File in(in_dir);
		if (!in.exists() || in.isFile())
		{
			std::cerr << "input dir doesnot exists" << std::endl;
			logger().information("input dir donnot exists");
			return -1;
		}

		auto modeljs = configFile().getString("facemodel");

		SperefaceFeature sphere;
		if(!sphere.init(modeljs)) {
			logger().information("init sphere model failed");
			return -1;
		}

		auto feature = [&sphere](const std::string & imgfile) -> decltype(auto) {
			auto mat = cv::imread(imgfile);
			return sphere.feature(mat);
		};

		Extracter extracter(feature, maxFolder, maxImg);

		auto ret = extracter.mkIndex(in_dir.toString(), indexfile.toString(), h5File.toString());

		logger().information("extract feature ok");

		return ret ? 0 : 1;
	}
};

POCO_APP_MAIN(DetectorApp)
