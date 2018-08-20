

#include "TmplApp.h"

using namespace Poco;
using namespace Poco::Util;

TmplApp::TmplApp() : _helpRequested(false)
{
}

void TmplApp::initialize(Application& self)
{
	loadConfiguration(); // load default configuration files, if present
	Application::initialize(self);
	// add your own initialization code here
}

void TmplApp::uninitialize()
{
	// add your own uninitialization code here
	Application::uninitialize();
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

