#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QListView>
#include <QHoverEvent>

class ImageItem : public QStandardItem {
public:

};

class ImageListModel : public QStandardItemModel
{
public:
    ImageListModel(QObject * p = nullptr);

public:

    void setupTestData();
};


class ImageListView : public QListView {
public:
    ImageListView(QWidget * p = nullptr);

protected:
    void leaveEvent(QEvent *) final
    {
        QHoverEvent hev(QEvent::HoverLeave, QPointF(), QPointF());
        viewportEvent(&hev); // Seemingly needed to kill the hover paint.
    }
};

#endif // IMAGELISTMODEL_H
