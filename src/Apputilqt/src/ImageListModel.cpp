#include <Cvn/Apputilqt/ImageListModel.h>
#include <QStyle>
#include <QTextOption>
#include "ImageItemDelegate.h"
#include <QAbstractItemView>
#include <QPainter>
#include <QTime>
#include <QPointer>
#include <QLabel>

ImageListModel::ImageListModel(QObject * p ) :
    QStandardItemModel(p)
{

}

void ImageListModel::setupTestData()
{
    QWidget w;
    QList<QIcon> icons;
    icons << w.style()->standardIcon(QStyle::SP_FileDialogStart);
    icons << w.style()->standardIcon(QStyle::SP_MediaPlay);
    icons << w.style()->standardIcon(QStyle::SP_MediaStop);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipForward);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipBackward);
    icons << w.style()->standardIcon(QStyle::SP_MediaVolume);
    icons << w.style()->standardIcon(QStyle::SP_FileDialogStart);
    icons << w.style()->standardIcon(QStyle::SP_MediaPlay);
    icons << w.style()->standardIcon(QStyle::SP_MediaStop);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipForward);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipBackward);
    icons << w.style()->standardIcon(QStyle::SP_MediaVolume);
    icons << w.style()->standardIcon(QStyle::SP_FileDialogStart);
    icons << w.style()->standardIcon(QStyle::SP_MediaPlay);
    icons << w.style()->standardIcon(QStyle::SP_MediaStop);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipForward);
    icons << w.style()->standardIcon(QStyle::SP_MediaSkipBackward);
    icons << w.style()->standardIcon(QStyle::SP_MediaVolume);

    for(auto & icon : icons) {
        auto item = new QStandardItem(icon, "TestData");
        this->appendRow(item);
    }
}


ImageListView::ImageListView(QWidget *p):
    QListView(p)
{
    this->setObjectName("grid");
    this->setViewMode(QListView::IconMode);
    this->setUniformItemSizes(true);
    this->setIconSize(QSize(230,230));
    this->setGridSize(QSize(230,230));
    this->setResizeMode(QListView::Adjust);
    this->setFocusPolicy(Qt::NoFocus);
    this->setTextElideMode(Qt::ElideMiddle);
    this->setMovement(QListView::Static);
    this->setItemDelegate(new ImageItemDelegate(this));

    setVerticalScrollMode(ScrollPerPixel);
    setMouseTracking(true); // To enable hover.
    setSelectionMode(QAbstractItemView::NoSelection);
    //setFrameShape(QFrame::NoFrame);
}
