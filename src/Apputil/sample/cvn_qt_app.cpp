

#include <Cvn/Apputil/QtApplication.h>
#include "AppWin.h"

class CvnApp : public Cvn::Apputil::QtApplication {
protected:
	void setupMainUi() {
		QtApplication::setupMainUi();
		setupMainWidget(new AppWin);
	}
};

int main(int argc, char** argv)
{
	Poco::AutoPtr<CvnApp> pApp = new CvnApp;

	try
	{
		pApp->init(argc, argv);
		Q_INIT_RESOURCE(resources);
		Q_INIT_RESOURCE(QtAwesome);
		Q_INIT_RESOURCE(style);

	}
	catch (Poco::Exception& exc)
	{
		pApp->logger().log(exc);
		return Poco::Util::Application::EXIT_CONFIG;
	}

	return pApp->run();
}