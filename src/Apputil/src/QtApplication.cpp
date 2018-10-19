//
// Created by tom on 16-11-12.
//

#include <Cvn/Apputil/QtApplication.h>
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <sstream>
#include <Cvn/Apputil/QtScheduler.h>
#include <QFile>
#include <QTextStream>
#include "AppWidget.h"

namespace Cvn
{
namespace Apputil
{

using namespace Poco::Util;

void QtApplication::init(int argc, char * argv[] )
{
    _argc = argc;
    _argv = argv;
    Application::init(argc, argv);
}

void QtApplication::initialize(Application& self)
{
    loadConfiguration();
    _app.reset(new QApplication(_argc, _argv));
    QtScheduler::instance();
	QFile f(":/qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}

    loadConfiguration(); // load default configuration files, if present
    Application::initialize(self);
}


void QtApplication::uninitialize()
{
    Application::uninitialize();
    _app.reset();
}

void QtApplication::reinitialize(Application &self) {
    Application::reinitialize(self);
}

void QtApplication::defineOptions(OptionSet& options)
{
    Application::defineOptions(options);

    options.addOption(
            Option("help", "h", "display help information on command line arguments")
                    .required(false)
                    .repeatable(false)
                    .callback(OptionCallback<QtApplication>(this, &QtApplication::handleHelp)));

    options.addOption(
            Option("define", "D", "define a configuration property")
                    .required(false)
                    .repeatable(true)
                    .argument("name=value")
                    .callback(OptionCallback<QtApplication>(this, &QtApplication::handleDefine)));

    options.addOption(
            Option("config-file", "f", "load configuration data from a file")
                    .required(false)
                    .repeatable(true)
                    .argument("file")
                    .callback(OptionCallback<QtApplication>(this, &QtApplication::handleConfig)));

    options.addOption(
            Option("bind", "b", "bind option value to test.property")
                    .required(false)
                    .repeatable(false)
                    .argument("value")
                    .binding("test.property"));
}

void QtApplication::handleHelp(const std::string& name, const std::string& value)
{
    _helpRequested = true;
    displayHelp();
    stopOptionsProcessing();
}

void QtApplication::handleDefine(const std::string& name, const std::string& value)
{
    defineProperty(value);
}

void QtApplication::handleConfig(const std::string& name, const std::string& value)
{
    loadConfiguration(value);
}

void QtApplication::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample application that demonstrates some of the features of the Poco::Util::Application class.");
    helpFormatter.format(std::cout);
}

void QtApplication::defineProperty(const std::string& def)
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

void QtApplication::printProperties(const std::string& base)
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

void QtApplication::setupMainWidget(QWidget * w) {
	poco_assert(w);
	poco_assert(_main);
	auto c = _main->centralWidget();
	_main->setCentralWidget(w);
	delete c;
}

void QtApplication::setupMainUi() {
    _main = std::make_shared<QMainWindow>();
    _main->resize(800, 600);
    _main->setCentralWidget(new AppWidget(_main.get()));
    _main->show();
}

void QtApplication::shotdownMainUi() {
    _main.reset();
}

int QtApplication::main(const std::vector<std::string>& args)
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
    }
    setupMainUi();
    _app->exec();
    shotdownMainUi();
    return Application::EXIT_OK;
}

}
}