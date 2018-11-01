
#include "AppWin.h"
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <qtmaterialappbar.h>
#include <qtmaterialiconbutton.h>
#include <qtmaterialtabs.h>
#include <lib/qtmaterialtheme.h>
#include <Cvn/Apputilqt/VPlayer.h>


#include <QtAwesome.h>

AppWin::AppWin(QWidget * parent) :QWidget(parent) {
	QVBoxLayout * layout = new QVBoxLayout(this);
	//layout->addWidget(new QLabel("top"));
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	auto tabs = new  QtMaterialTabs(this);

	QtAwesome* awesome = new QtAwesome(this);
	awesome->initFontAwesome();

	{
		QVariantMap options;
		tabs->addTab("Media", awesome->icon(fa::beer, options));
	}

	{
		QVariantMap options;
		options.insert("color", QColor(Qt::green));
		options.insert("text-off", QString(fa::squareo));
		options.insert("color-off", QColor(Qt::red));
		tabs->addTab("Playback", awesome->icon(fa::checksquareo, options));
	}
	tabs->addTab("Audio");
	tabs->addTab("Video");
	tabs->addTab("Tools");
	tabs->setMaximumHeight(100);

	layout->addWidget(tabs);

	layout->addWidget(new Cvn::Apputil::VPlayer(this));
}