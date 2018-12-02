#ifndef IMAGEITEMDELEGATE_H
#define IMAGEITEMDELEGATE_H


#include <QStyle>
#include <QPointer>
#include <QTimer>
#include <QPainter>
#include <QIcon>
#include <QTextOption>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QTime>

class ImageItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    ImageItemDelegate(QObject *parent = nullptr);

    void goon()
    {
        if (m_currentWidget)
            m_currentWidget->viewport()->update(m_currentArea);
    }

    bool editorEvent(QEvent *ev, QAbstractItemModel *model,
        const QStyleOptionViewItem &option, const QModelIndex &idx) final;


    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const final;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;

signals:
    void tagClicked(const QString &tag);

private:
    const QColor lightColor = QColor(221, 0, 220); // color: "#dddcdc"
    const QColor backgroundColor = QColor(0, 240, 0);
    const QColor foregroundColor1 = QColor(127, 0, 0); // light-ish.
    const QColor foregroundColor2 = QColor(200, 0, 0); // blacker.

    mutable QPersistentModelIndex m_previousIndex;
    mutable QTime m_startTime;
    mutable QRect m_currentArea;
    mutable QPointer<QAbstractItemView> m_currentWidget;
    mutable QVector<QPair<QString, QRect>> m_currentTagRects;
};

#endif // IMAGEITEMDELEGATE_H
