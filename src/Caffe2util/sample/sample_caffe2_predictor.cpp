// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Cvn/Apputil/TmplApp.h>
#include <nlohmann/json.hpp>
#include <Cvn/Caffe2util/CPredictor.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <memory>

using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;

class PredictorApp : public TmplApp {
public:
      const json sample_json =
          {
              {"dir", "/home/tony/data/ws.cv.face.model/caffe2/sphereface/"},
              {"predict_net", "sphereface_20_predict_net.pb"},
              {"init_net",      "sphereface_20_init_net.pb"},
              {"input_names", {"data"}},
              {"output_names", {"fc5"}}};
	virtual void dumpParamList(std::ostream& ostr) const {
		ostr << sample_json.dump() << std::endl;
	}

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

        CPredictor predictor;
		bool ok = predictor.load(sample_json.dump());
		if(!ok) {
			logger().information("load json failed");
			logger().information(sample_json.dump());
			return -1;
		} else {
			logger().information("load caffe2 model ok");
		}
		//logger().information("para.answer.evertything=" + config().getRawString("para.list[1]"));
		return 0;
	}
};


POCO_APP_MAIN(PredictorApp)
