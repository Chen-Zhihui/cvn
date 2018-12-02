
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

#include <Cvn/Apputilqt/DirView.h>
#include <Cvn/Apputilqt/QtRx.h>
#include <Cvn/Apputilqt/Player.h>
#include <Cvn/Apputilqt/ImageViewer.h>
#include <Cvn/Apputilqt/ImageListModel.h>

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
	QtAwesome* awesome = new QtAwesome(this);
	awesome->initFontAwesome();

	QVBoxLayout * layout = new QVBoxLayout(this);
	//layout->addWidget(new QLabel("top"));
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	auto m_appBar = new QtMaterialAppBar(this);
	layout->addWidget(m_appBar);
    QtMaterialIconButton *button = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
    button->setIconSize(QSize(24, 24));
    button->setColor(Qt::white);
    button->setFixedWidth(42);
    m_appBar->appBarLayout()->addWidget(button);

	QLabel *label = new QLabel("Inbox");
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);
    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);
    label->setFont(QFont("Roboto", 18, QFont::Normal));
    m_appBar->appBarLayout()->addWidget(label);

    //m_appBar->appBarLayout()->addStretch(1);

	auto tabs = new  QtMaterialTabs(this);
	m_appBar->appBarLayout()->addWidget(tabs);
    {
        QVariantMap options;
        tabs->addTab("Media", awesome->icon(fa::beer, options));
    }

    {
        QVariantMap options;
        options.insert("color", QColor(Qt::green));
        options.insert("text-off", QString(fa::squareo));
        options.insert("color-off", QColor(Qt::red));
        tabs->addTab("DirView", awesome->icon(fa::checksquareo, options));
    }
    {
        QVariantMap options;
        tabs->addTab("Media", awesome->icon(fa::beer, options));
    }
    {
        QVariantMap options;
        tabs->addTab("Media", awesome->icon(fa::beer, options));
    }

    {
        QVariantMap options;
        options.insert("color", QColor(Qt::green));
        options.insert("text-off", QString(fa::squareo));
        options.insert("color-off", QColor(Qt::red));
        tabs->addTab("DirView", awesome->icon(fa::checksquareo, options));
    }
	tabs->setMaximumHeight(100);

	auto stacked = new QStackedWidget(this);
    layout->addWidget(stacked);

    auto view = new ImageListView;
    ImageListModel * model= new ImageListModel;
    model->setupTestData();
    view->setModel(model);
    stacked->addWidget(view);
    //view->show();


	auto player = new Player(this);
	stacked->addWidget(player);
	stacked->addWidget(new Cvn::Apputil::DirView());
    stacked->addWidget(new ImageViewer());


/*
	auto hlayout = new QHBoxLayout();
	layout->addLayout(hlayout);
	auto left = new QLabel(this);
	auto middle = new QLabel(this);
	auto right = new QLabel(this);
	hlayout->addWidget(left);
	hlayout->addWidget(middle);
	hlayout->addWidget(right);
	layout->addLayout(hlayout);

	auto threadId= [](const QString& name) -> QString 	{
		return name + QString(": %1\n").arg((long long)QThread::currentThreadId());
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
	*/


	connect(tabs, SIGNAL(currentChanged(int)), stacked, SLOT(setCurrentIndex(int)));
}