
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

#include <rxqt.hpp>
#include <rxcpp/operators/rx-subscribe_on.hpp>
#include <rxcpp/operators/rx-observe_on.hpp>

#include <rxqt.hpp>
#include <QTime>

namespace rx {
	using namespace rxcpp;
	using namespace rxcpp::sources;
	using namespace rxcpp::operators;
	using namespace rxcpp::schedulers;
	using namespace rxcpp::util;
}

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

	auto player = new Cvn::Apputil::VPlayer(this);

	layout->addWidget(player);

	auto hlayout = new QHBoxLayout(this);
	auto left = new QLabel(this);
	auto right = new QLabel(this);
	hlayout->addWidget(left);
	hlayout->addWidget(right);
	layout->addLayout(hlayout);

	rxqt::from_signal(player, &Cvn::Apputil::VPlayer::present)
		.map([](const QImage & s) { return QTime::currentTime().toString(); })
		.subscribe([left](const QString& s) { left->setText(s); });
}