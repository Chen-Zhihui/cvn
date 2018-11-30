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

    void goon()
    {
        if (m_currentWidget)
            m_currentWidget->viewport()->update(m_currentArea);
    }

    bool editorEvent(QEvent *ev, QAbstractItemModel *model,
        const QStyleOptionViewItem &option, const QModelIndex &idx) final
    {
    //        if (ev->type() == QEvent::MouseButtonRelease) {
    //            const ExampleItem item = idx.data(Qt::UserRole).value<ExampleItem>();
    //            auto mev = static_cast<QMouseEvent *>(ev);
    //            if (idx.isValid()) {
    //                const QPoint pos = mev->pos();
    //                if (pos.y() > option.rect.y() + tagsSeparatorY) {
    //                    //const QStringList tags = idx.data(Tags).toStringList();
    //                    for (auto it : m_currentTagRects) {
    //                        if (it.second.contains(pos))
    //                            emit tagClicked(it.first);
    //                    }
    //                } else {
    //                    if (item.isVideo)
    //                        QDesktopServices::openUrl(QUrl::fromUserInput(item.videoUrl));
    //                    else if (item.hasSourceCode)
    //                        ExamplesWelcomePage::openProject(item);
    //                    else
    //                        HelpManager::handleHelpRequest(QUrl::fromUserInput(item.docUrl),
    //                                                       HelpManager::ExternalHelpAlways);
    //                }
    //            }
    //        }
        return QAbstractItemDelegate::editorEvent(ev, model, option, idx);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const final;

    void setShowExamples(bool showExamples) { m_showExamples = showExamples; goon(); }

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
    bool m_showExamples = true;
};

#endif // IMAGEITEMDELEGATE_H
