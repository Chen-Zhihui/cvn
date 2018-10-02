

#include <Cvn/Apputil/TmplApp.h>
#include <Poco/Util/ConfigurationView.h>
#include <Poco/Util/JSONConfiguration.h>
#include <Poco/Util/ConfigurationMapper.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Format.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <iostream>
#include <nlohmann/json.hpp>

namespace Cvn {
	namespace Apputil {

	using json = nlohmann::json;
	using namespace Poco;
	using namespace Poco::Util;

	void logConfig(const std::string &prefix, AbstractConfiguration & config, Logger & logger, int depth = 0) {
		AbstractConfiguration::Keys keys;
		config.keys(keys);
		std::string space("\t");
		std::string spaceN;
		for (int i = 0; i < depth; i++) {
			space += "\t";
			spaceN += "\t";
		}
		logger.information(spaceN + (prefix == "" ? "{" : prefix + ": {"));
		for (auto & key : keys) {
			if (config.has(key)) {
				auto msg = space + key + " : " + config.getRawString(key);
				logger.information(msg);
				//std::cout << msg << std::endl;
			}
			else {
				AutoPtr<ConfigurationView> view(new ConfigurationView(key, &config));
				logConfig(key, *view, logger, depth + 1);
			}
		}
		logger.information(spaceN + "}");
	}

	TmplApp::TmplApp() : _helpRequested(false)
	{
		setUnixOptions(true);
		_pMapConfig = new MapConfiguration;
		auto & cnf = config();
		const_cast<LayeredConfiguration&>(cnf).add(_pMapConfig, PRIO_DEFAULT, false, false);


		_pJsonConfig = new JSONConfiguration();
		auto mapper = new ConfigurationMapper("", "para", _pJsonConfig);
		const_cast<LayeredConfiguration&>(cnf).add(mapper, PRIO_DEFAULT - 1, false, false);
	}

	void TmplApp::setupLogger() {

		poco_assert_msg(config().has("application.dataDir"), "update poco libs, make sure config before init");
		auto appName = config().getString("application.baseName");
		if(appName.empty()) {

		}

		Path logfile(config().getString("system.currentDir"));
		logfile.append(appName + ".log");
		logfile.makeFile();

		AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());

		//AutoPtr<Channel> consoleChannel(new ConsoleChannel());
		//splitterChannel->addChannel(consoleChannel);
		AutoPtr<FileChannel> rotatedFileChannel(new FileChannel(logfile.toString()));
		rotatedFileChannel->setProperty("rotation", "10M");
		rotatedFileChannel->setProperty("archive", "timestamp");
		splitterChannel->addChannel(rotatedFileChannel);

		AutoPtr<Formatter> formatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i %s: %t"));
		AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));

		Logger::setChannel("", formattingChannel);

		for (int i = 0; i < 10; i++)
			logger().information("###################################################");
		    logger().information("^^^^^^^^^^^^^^^^^^^^ start ^^^^^^^^^^^^^^^^^^^^^^^^");
	}

	void TmplApp::init(int argc, char* argv[]) {
		std::stringstream os;
		dumpParamList(os);
		_pJsonConfig->load(os);

		Application::init(argc, argv);

		setupLogger();

		logger().information("default application settings");
		logConfig("", config(), this->logger());
	}

#if defined(POCO_WIN32_UTF8) && !defined(POCO_NO_WSTRING)
	void TmplApp::init(int argc, wchar_t* argv[]) {
		std::stringstream os;
		dumpParamList(os);
		_pJsonConfig->load(os);

		Application::init(argc, argv);

		setupLogger();
		
		logger().information("==== application settings with command line args");
		logConfig("", config(), this->logger());
	}
#endif

	void TmplApp::handleOption(const std::string& name, const std::string& value) {
		Application::handleOption(name, value);
	}

	void TmplApp::initialize(Application& self)
	{
		logger().information("====================== initialize ==========================");
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		// add your own initialization code here
	}

	void TmplApp::uninitialize()
	{
		// add your own uninitialization code here
		Application::uninitialize();

		logger().information("====================== end ==========================");
	}

	void TmplApp::reinitialize(Application& self)
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
	}

	void TmplApp::defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<TmplApp>(this, &TmplApp::handleHelp)));

		options.addOption(
			Option("define", "D", "define a configuration property")
			.required(false)
			.repeatable(true)
			.argument("name=value")
			.callback(OptionCallback<TmplApp>(this, &TmplApp::handleDefine)));

		options.addOption(
			Option("param", "P", "define a param in param list")
			.required(false)
			.repeatable(true)
			.argument("name=value")
			.callback(OptionCallback<TmplApp>(this, &TmplApp::handleParameter)));

		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
			.required(false)
			.repeatable(true)
			.argument("file")
			.callback(OptionCallback<TmplApp>(this, &TmplApp::handleConfig)));

		options.addOption(
			Option("param-list", "l", "default parameter list as json")
			.repeatable(false)
			.required(false)
			.callback(OptionCallback<TmplApp>(this, &TmplApp::handleParamList))
		);

		logger().information("==== options");
		for (auto & opt : options) {
			logger().information(opt.fullName() + " : " + " " + opt.argumentName() + " : " + opt.description());
		}
	}

	void TmplApp::handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		//stopOptionsProcessing();
	}

	void TmplApp::handleDefine(const std::string& name, const std::string& value)
	{
		defineProperty(value);
	}

	void TmplApp::handleParameter(const std::string & name, const std::string & value) {
		defineProperty(value, *_pJsonConfig);
	}

	void TmplApp::handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}

	void TmplApp::handleParamList(const std::string & name, const std::string & value) {

		_helpRequested = true;
		dumpParamList(std::cout);
	}
	void TmplApp::displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setUnixStyle(true);
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
		helpFormatter.format(std::cout);
	}

	void TmplApp::defineProperty(const std::string& def)
	{
		defineProperty(def, *_pMapConfig);
	}


	void TmplApp::defineProperty(const std::string & def, AbstractConfiguration & conf) {
		std::string name;
		std::string value;
		std::string::size_type pos = def.find('=');
		if (pos != std::string::npos)
		{
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		conf.setString(name, value);
	}

	int TmplApp::main(const ArgVec& args)
	{
		if (!_helpRequested)
		{
			logger().information("Command line:");
			std::ostringstream ostr;
			for (ArgVec::const_iterator it = argv().begin(); it != argv().end(); ++it)
			{
				ostr << *it << ' ';
			}
			logger().information(ostr.str());
			logger().information("Arguments to main():");
			for (ArgVec::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				logger().information(*it);
			}
			logger().information("Application properties:");
			printConfig(config());
		}
		else {
			logger().information("command line ok");
		}
		return Application::EXIT_OK;
	}

	void TmplApp::printProperties(const std::string& base)
	{
		AbstractConfiguration::Keys keys;
		config().keys(base, keys);
		if (keys.empty())
		{
			if (config().hasProperty(base))
			{
				std::string msg;
				msg.append(base);
				msg.append(" = ");
				msg.append(config().getString(base));
				logger().information(msg);
			}
		}
		else
		{
			for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
			{
				std::string fullKey = base;
				if (!fullKey.empty()) fullKey += '.';
				fullKey.append(*it);
				printProperties(fullKey);
			}
		}
	}


	void TmplApp::printConfig(Poco::Util::AbstractConfiguration & conf) {
		logConfig(std::string(), conf, logger());
	}

	void TmplApp::dumpParamList(std::ostream& ostr) const {
		auto j2 = R"(
					  {
					  }
					)"_json;
		ostr << j2.dump(4) << std::endl;
	}

}
}