
#include "AppWin.h"
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <qtmaterialiconbutton.h>
#include <lib/qtmaterialtheme.h>
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
	QtAwesome* awesome = new QtAwesome(this);
	awesome->initFontAwesome();

	QVBoxLayout * layout = new QVBoxLayout(this);
    {
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    appbar = new QtMaterialAppBar(this);
    {
        layout->addWidget(appbar);
        QtMaterialIconButton *button = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
        button->setIconSize(QSize(24, 24));
        button->setColor(Qt::white);
        button->setFixedWidth(42);
        appbar->appBarLayout()->addWidget(button);

        QLabel *label = new QLabel("Inbox");
        label->setAttribute(Qt::WA_TranslucentBackground);
        label->setForegroundRole(QPalette::Foreground);
        label->setContentsMargins(6, 0, 0, 0);
        QPalette palette = label->palette();
        palette.setColor(label->foregroundRole(), Qt::white);
        label->setPalette(palette);
        label->setFont(QFont("Roboto", 18, QFont::Normal));
        appbar->appBarLayout()->addWidget(label);
    }

    apptab = new  QtMaterialTabs(this);
    {
        appbar->appBarLayout()->addWidget(apptab);
        apptab->setMaximumHeight(100);
    }

	stacked = new QStackedWidget(this);
    layout->addWidget(stacked);
    connect(apptab, SIGNAL(currentChanged(int)), stacked, SLOT(setCurrentIndex(int)));

    //page one
    {
        detectorPage = new QSplitter(Qt::Vertical, stacked);
        stacked->addWidget(detectorPage);
        {
            QVariantMap options;
            apptab->addTab("Player", awesome->icon(fa::beer, options));
        }

        fsView = new DirView(detectorPage);
        player = new Player(detectorPage);
        detView = new ImageListView(detectorPage);
        detView->setFlow(QListView::LeftToRight);
        monitor = new ImageViewer(detectorPage);
        {
            auto up = new QSplitter(Qt::Horizontal,detectorPage);
            auto down = new QSplitter(Qt::Horizontal,detectorPage);
            detectorPage->addWidget(up);
            detectorPage->addWidget(down);
            up->addWidget(fsView);
            up->addWidget(player);
            down->addWidget(monitor);
            down->addWidget(detView);
        }
    }

    //page two
    {
        searchPage = new QSplitter(Qt::Horizontal,stacked);
        stacked->addWidget(searchPage);
        {
            QVariantMap options;
            options.insert("color", QColor(Qt::green));
            options.insert("text-off", QString(fa::squareo));
            options.insert("color-off", QColor(Qt::red));
            apptab->addTab("Search", awesome->icon(fa::checksquareo, options));
        }
        byNameView = new ImageListView(searchPage);
        byImageView = new ImageListView(searchPage);
        imageView = new ImageViewer(searchPage);
        dbPanel = new QWidget(searchPage);
        {
            searchPage->addWidget(byImageView);
            auto vlayout = new QSplitter(Qt::Vertical, searchPage);
            vlayout->setMinimumWidth(300);
            vlayout->addWidget(imageView);
            vlayout->addWidget(dbPanel);
            searchPage->addWidget(byNameView);
        }
    }

    {
        settings = new QSplitter(Qt::Horizontal, stacked);
        stacked->addWidget(settings);
        {
            QVariantMap options;
            options.insert("color", QColor(Qt::green));
            options.insert("text-off", QString(fa::home));
            options.insert("color-off", QColor(Qt::red));
            apptab->addTab("Settings", awesome->icon(fa::home, options));
        }
    }
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


}