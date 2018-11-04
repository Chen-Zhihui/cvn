
#include <QApplication>
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <qtmaterialappbar.h>
#include <qtmaterialiconbutton.h>
#include <qtmaterialtabs.h>
#include <lib/qtmaterialtheme.h>

int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(resources);

	QWidget * mainwdg = new QWidget;
	mainwdg->resize(1000, 800);

	QVBoxLayout * layout = new QVBoxLayout(mainwdg);
	layout->setContentsMargins(0, 0, 0, 0);

	auto tabs = new  QtMaterialTabs();
	tabs->addTab("Media");
	tabs->addTab("Playback");
	tabs->addTab("Audio");
	tabs->addTab("Video");
	tabs->addTab("Tools");

	layout->addWidget(tabs);
	layout->setAlignment(tabs, Qt::AlignTop);

	layout->addWidget(new QStackedWidget);

	mainwdg->show();

	return app.exec();
}