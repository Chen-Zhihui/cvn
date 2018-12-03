
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
#include <Cvn/Apputilqt/DirView.h>
#include <Cvn/Apputilqt/QtRx.h>
#include <QtAwesome.h>

#include <rxqt.hpp>
#include <rxcpp/operators/rx-subscribe_on.hpp>
#include <rxcpp/operators/rx-observe_on.hpp>

#include <rxqt.hpp>
#include <QTime>
#include <QThread>
#include <QDebug>

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
	tabs->addTab("DirView");
	tabs->addTab("Video");
	tabs->addTab("Tools");
	tabs->setMaximumHeight(100);

	layout->addWidget(tabs);

	auto stacked = new QStackedWidget(this);
	auto player = new Cvn::Apputil::VPlayer(this);
	stacked->addWidget(player);
	stacked->addWidget(new DirView());

	layout->addWidget(stacked);



	auto hlayout = new QHBoxLayout(this);
	auto left = new QLabel(this);
	auto middle = new QLabel(this);
	auto right = new QLabel(this);
	hlayout->addWidget(left);
	hlayout->addWidget(middle);
	hlayout->addWidget(right);
	layout->addLayout(hlayout);

	auto threadId= [](const QString& name) -> QString 	{
		return name + QString(": %1\n").arg((int)QThread::currentThreadId());
	};

	rxqt::from_signal(player, &Cvn::Apputil::VPlayer::present)
		.subscribe_on(rx::observe_on_new_thread())
		.map([](const QImage & s) -> QString { return QTime::currentTime().toString(); })
		.tap([threadId](const QString & msg) { qDebug() << threadId(QString("subscrive_on %1").arg(msg));  })
		.observe_on(ui_thread())
		.tap([right, threadId](const QString & str) { right->setText(threadId(QString("subscrive_on %1").arg(str) )); })
		.subscribe([middle, threadId](const QString & str) { middle->setText(threadId(QString("observe_on %1").arg(str))); });

	rxqt::from_event(player, QEvent::KeyPress)
		.subscribe([left](const QEvent* e) {
		auto ke = static_cast<const QKeyEvent*>(e);
		qDebug() << ke->key();
		left->setText(QString("KeyEvent=%1").arg(ke->key()));
	});
	


	connect(tabs, SIGNAL(currentChanged(int)), stacked, SLOT(setCurrentIndex(int)));
}