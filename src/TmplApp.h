
#pragma once

#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/AutoPtr.h>
#include <iostream>
#include <sstream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;

class TmplApp : public Poco::Util::Application {

    public:
        
    TmplApp();

	void init(int argc, char* argv[]);

#if defined(POCO_WIN32_UTF8) && !defined(POCO_NO_WSTRING)
	void init(int argc, wchar_t* argv[]);
#endif

protected:


	void initialize(Poco::Util::Application& self);

	void uninitialize();

	void reinitialize(Poco::Util::Application & self);

	void defineOptions(Poco::Util::OptionSet & options);

	void handleHelp(const std::string & name, const std::string & value);

	void handleDefine(const std::string & name, const std::string & value);

	void handleConfig(const std::string & name, const std::string & value);

	void displayHelp();

	void defineProperty(const std::string & def);

	int main(const Poco::Util::Application::ArgVec & args);

	void printProperties(const std::string & base);

protected:

	virtual void handleOption(const std::string& name, const std::string& value) override;

private:

	void setupLogger();

	bool _helpRequested = false;

	Poco::Util::MapConfiguration * _pMapConfig = nullptr;

};