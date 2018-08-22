

#include "TmplApp.h"
#include <Poco/Util/ConfigurationView.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Format.h>
#include <Poco/File.h>
#include <iostream>

using namespace Poco;
using namespace Poco::Util;

void logConfig(const std::string &prefix, AbstractConfiguration & config, Logger & logger) {
	AbstractConfiguration::Keys keys;
	config.keys(keys);
	for (auto & key : keys) {
		if (config.has(key)) {
			auto msg = (prefix == "" ? std::string("") : prefix + ".") + key + " = " + config.getRawString(key);
			logger.information(msg);
			//std::cout << msg << std::endl;
		} else {
			auto branch = prefix == "" ? key : prefix + "." + key;
			AutoPtr<ConfigurationView> view( new ConfigurationView( key, &config) );
			logConfig(branch, *view, logger);
		}
	}
}

TmplApp::TmplApp() : _helpRequested(false)
{
	_pMapConfig = new MapConfiguration;
	auto & cnf = config();
	const_cast<LayeredConfiguration&>(cnf).add( _pMapConfig, PRIO_DEFAULT, false, false);
	setupLogger();
}

void TmplApp::setupLogger() {

	poco_assert_msg(config().has("application.dataDir"), "update poco libs, make sure config before init");
	
	auto appDataDir = config().getString("application.dataDir");
	auto appName = config().getString("application.baseName");

	File appdir(appDataDir);
	appdir.createDirectories();

	AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());

	//AutoPtr<Channel> consoleChannel(new ConsoleChannel());
	//splitterChannel->addChannel(consoleChannel);
	AutoPtr<FileChannel> rotatedFileChannel(new FileChannel(appDataDir + appName + ".log"));
	rotatedFileChannel->setProperty("rotation", "100000");
	rotatedFileChannel->setProperty("archive", "timestamp");
	splitterChannel->addChannel(rotatedFileChannel);

	AutoPtr<Formatter> formatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i %s: %t"));
	AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));

	Logger::setChannel("", formattingChannel);

	logger().information("^^^^^^^^^^^^^^^^^^^^ start ^^^^^^^^^^^^^^^^^^^^^^^^");
}

void TmplApp::init(int argc, char* argv[]) {
	logger().information("default application settings");
	logConfig("", config(), this->logger());

	Application::init(argc, argv);

	logger().information("default application settings");
	logConfig("", config(), this->logger());
}

#if defined(POCO_WIN32_UTF8) && !defined(POCO_NO_WSTRING)
void TmplApp::init(int argc, wchar_t* argv[]) {
	logger().information("application settings : default");
	logConfig("", config(), this->logger());

	Application::init(argc, argv);

	logger().information("application settings with command line args");
	logConfig("", config(), this->logger());
}
#endif

void TmplApp::handleOption(const std::string& name, const std::string& value) {
	_pMapConfig->setString(name, value);	
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
		Option("config-file", "f", "load configuration data from a file")
		.required(false)
		.repeatable(true)
		.argument("file")
		.callback(OptionCallback<TmplApp>(this, &TmplApp::handleConfig)));

	options.addOption(
		Option("bind", "b", "bind option value to test.property")
		.required(false)
		.repeatable(false)
		.argument("value")
		.binding("test.property"));
}

void TmplApp::handleHelp(const std::string& name, const std::string& value)
{
	_helpRequested = true;
	displayHelp();
	stopOptionsProcessing();
}

void TmplApp::handleDefine(const std::string& name, const std::string& value)
{
	defineProperty(value);
}

void TmplApp::handleConfig(const std::string& name, const std::string& value)
{
	loadConfiguration(value);
}

void TmplApp::displayHelp()
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.setUsage("OPTIONS");
	helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
	helpFormatter.format(std::cout);
}

void TmplApp::defineProperty(const std::string& def)
{
	std::string name;
	std::string value;
	std::string::size_type pos = def.find('=');
	if (pos != std::string::npos)
	{
		name.assign(def, 0, pos);
		value.assign(def, pos + 1, def.length() - pos);
	}
	else name = def;
	config().setString(name, value);
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
		printProperties("");
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

