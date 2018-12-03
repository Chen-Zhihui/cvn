
#include <QMainWindow>
#include <QSplitter>
#include <QFileSystemModel>
#include <rxqt.hpp>
#include <QStackedWidget>
#include <Cvn/Apputilqt/DirView.h>
#include <Cvn/Apputilqt/Player.h>
#include <Cvn/Apputilqt/ImageViewer.h>
#include <Cvn/Apputilqt/ImageListModel.h>
#include <qtmaterialappbar.h>
#include <qtmaterialtabs.h>


class AppWin : public QWidget {
public:

	AppWin(QWidget * parent = nullptr);

	//rxqt::run_loop rxqt_run_loop;

public:

    QtMaterialAppBar * appbar = nullptr;
    QtMaterialTabs * apptab = nullptr;
    QStackedWidget * stacked = nullptr;

    //page one
	QSplitter * detectorPage = nullptr;
	DirView * fsView = nullptr;

    Player * player = nullptr;
    ImageListView * detView = nullptr;
    ImageListModel detList;
	ImageViewer * monitor = nullptr;

	//page two
	QSplitter * searchPage = nullptr;
    ImageListView * byNameView = nullptr;
    ImageListModel byNameList;
    ImageListView * byImageView = nullptr;
    ImageListModel byImageList;
    ImageViewer * imageView = nullptr;
    QWidget * dbPanel = nullptr;

    //page last
    QSplitter * settings = nullptr;




	// monitor
	// player
	// detector
	// byName
	// byPic
	// head

protected:


};