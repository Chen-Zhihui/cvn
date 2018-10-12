

#include <Cvn/Apputil/QtApplication.h>

int main(int argc, char** argv)
{
	Poco::AutoPtr<Cvn::Apputil::QtApplication> pApp = new Cvn::Apputil::QtApplication;
	try
	{
		pApp->init(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
		pApp->logger().log(exc);
		return Poco::Util::Application::EXIT_CONFIG;
	}
	return pApp->run();
}