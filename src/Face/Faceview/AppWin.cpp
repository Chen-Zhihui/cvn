
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
#include <QApplication>
#include <rxqt.hpp>
#include <rxcpp/operators/rx-subscribe_on.hpp>
#include <rxcpp/operators/rx-observe_on.hpp>
#include <qsettingsdialog.h>
#include <qsettingssettingsloader.h>

#include <rxqt.hpp>
#include <QTime>
#include <QThread>
#include <QDebug>

namespace rx
{
using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::operators;
using namespace rxcpp::schedulers;
using namespace rxcpp::util;
} // namespace rx

AppWin::AppWin(QWidget *parent) : QWidget(parent)
{

	QSettings settings(QApplication::applicationDirPath() + "/test.ini", QSettings::IniFormat);

    QtAwesome *awesome = new QtAwesome(this);
    awesome->initFontAwesome();

    QVBoxLayout *layout = new QVBoxLayout(this);
    {
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    appbar = new QtMaterialAppBar(this);
    {
        layout->addWidget(appbar);
		if (0) {
			QtMaterialIconButton *button = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
			button->setIconSize(QSize(24, 24));
			button->setColor(Qt::white);
			button->setFixedWidth(42);
			appbar->appBarLayout()->addWidget(button);
		}
		if (0) {
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
    }

    apptab = new QtMaterialTabs(this);
    {
        appbar->appBarLayout()->addWidget(apptab);
        apptab->setMaximumHeight(100);
    }

	auto settingBtn = new QtMaterialIconButton(awesome->icon(fa::home));
	{
		settingBtn->setIconSize(QSize(24, 24));
		settingBtn->setColor(Qt::white);
		settingBtn->setFixedWidth(42);
	}
	connect(settingBtn, &QtMaterialIconButton::clicked, [this]() {
		showSettings();
	});
	appbar->appBarLayout()->addWidget(settingBtn);


    stacked = new QStackedWidget(this);
    layout->addWidget(stacked);
    connect(apptab, SIGNAL(currentChanged(int)), stacked, SLOT(setCurrentIndex(int)));

    //page one
    {
        detectorPage = new QSplitter(Qt::Vertical, stacked);
        stacked->addWidget(detectorPage);
        {
            apptab->addTab("", awesome->icon(fa::filemovieo));
        }

        fsView = new DirView(detectorPage);
        player = new Player(detectorPage);
        detView = new ImageListView(detectorPage);
        detView->setFlow(QListView::LeftToRight);
        monitor = new ImageViewer(detectorPage);
        {
            auto up = new QSplitter(Qt::Horizontal, detectorPage);
            auto down = new QSplitter(Qt::Horizontal, detectorPage);
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
        searchPage = new QSplitter(Qt::Horizontal, stacked);
        stacked->addWidget(searchPage);
        {
            apptab->addTab("", awesome->icon(fa::checksquareo));
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

void AppWin::showSettings() const {


	QSettings settings(QApplication::applicationDirPath() + "/test.ini", QSettings::IniFormat);

		QSettingsDialog dialog;
		dialog.appendEntry(new QSettingsEntry(QMetaType::QString,
			new QSettingsSettingsLoader(&settings, "appName"),
			"App name"));
		dialog.appendEntry(new QSettingsEntry(QMetaType::QString,
			new QSettingsSettingsLoader(&settings, "companyName"),
			"Company name"));

		dialog.setGroup("versionGroup", 0, "Version", true, "Please configure the version");
		dialog.appendEntry(new QSettingsEntry(QMetaType::Int,
			new QSettingsSettingsLoader(&settings, "version/major"),
			"Major",
			false,
			QString(),
			{ {"minimum", 0}, {"maximum", 9} }));
		dialog.appendEntry(new QSettingsEntry(QMetaType::Int,
			new QSettingsSettingsLoader(&settings, "version/minor"),
			"Minor",
			false,
			QString(),
			{ {"minimum", 0}, {"maximum", 9} }));
		dialog.appendEntry(new QSettingsEntry(QMetaType::Int,
			new QSettingsSettingsLoader(&settings, "version/patch"),
			"Patch",
			false,
			QString(),
			{ {"minimum", 0}, {"maximum", 9} }));

		dialog.unsetGroup();
		dialog.appendEntry(new QSettingsEntry(QMetaType::QString,
			new QSettingsSettingsLoader(&settings, "authorName"),
			"Load Programmer",
			true));

		dialog.setSection("more", "More Stuff");
		dialog.appendEntry(new QSettingsEntry(QMetaType::Bool,
			new QSettingsSettingsLoader(&settings, "allow/A"),
			"Allow Option A"));
		dialog.appendEntry(new QSettingsEntry(QMetaType::Bool,
			new QSettingsSettingsLoader(&settings, "allow/B"),
			"Allow Option B"));
		dialog.appendEntry(new QSettingsEntry(QMetaType::Bool,
			new QSettingsSettingsLoader(&settings, "allow/C"),
			"Allow Option C"));

		dialog.setCategory("secret", "Secret Secure Settings", QIcon(":/QSettingsDialog/icons/gearSettings.ico"));
		dialog.setSection("."); //will not be required in next version
		dialog.appendEntry(new QSettingsEntry(QMetaType::QUrl,
			new QSettingsSettingsLoader(&settings, "secretUrl"),
			"Very secret url"));
		dialog.appendEntry(new QSettingsEntry(QMetaType::QColor,
			new QSettingsSettingsLoader(&settings, "secretColor"),
			"Very secret color"));

		dialog.openSettings();	
}