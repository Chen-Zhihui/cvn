// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Cvn/Apputil/TmplApp.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace Cvn::Apputil;

class ExampleApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 = {
		  {"pi", 3.141},
		  {"happy", true},
		  {"name", "Niels"},
		  {"nothing", nullptr},
		  {"answer", {
			{"everything", 42}
		  }},
		  {"list", {1, 0, 2}},
		  {"object", {
			{"currency", "USD"},
			{"value", 42.99}
		  }}
		};
		ostr << j2.dump() << std::endl;
	}


	int main(const Poco::Util::Application::ArgVec & args) {
		logger().information("Hello world");
		printConfig(this->configFile());
		logger().information("para.answer.evertything=" + config().getRawString("para.list[1]"));
		return 0;
	}
};


POCO_APP_MAIN(ExampleApp)
