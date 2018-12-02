
#include "ImageItemDelegate.h"

#include <QFont>

static QColor themeColor()
{
    return QColor(0, 0, 0, 127);
}

static QFont sizedFont(int size, const QWidget *widget, bool underline = false)
{
    QFont f = widget->font();
    f.setPixelSize(size);
    f.setUnderline(underline);
    return f;
}

const int itemWidth = 230;
const int itemHeight = 230;
const int itemGap = 10;
const int tagsSeparatorY = itemHeight - 60;
const QSize exampleImageSize(188, 145);

bool ImageItemDelegate::editorEvent(QEvent *ev, QAbstractItemModel *model,
                 const QStyleOptionViewItem &option, const QModelIndex &idx) {
    return QStyledItemDelegate::editorEvent(ev, model, option, idx);
}


void ImageItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    auto itemName = index.data().toString();
    auto itemTitle = QString("itemTitle");
    auto itemPixmap = index.data(Qt::DecorationRole).value<QIcon>().pixmap(exampleImageSize);
    auto itemTags = QString("itemTags");
    auto itemDesc = QString("itemDesc");
    const QRect rc = option.rect;

    const int d = 10;
    const int x = rc.x() + d;
    const int y = rc.y() + d;
    const int w = rc.width() - 2 * d - itemGap;
    const int h = rc.height() - 2 * d - itemGap;

    const bool hovered = option.state & QStyle::State_MouseOver;

    const int tagsBase = tagsSeparatorY + 10;
    const int shiftY = tagsSeparatorY - 20;
    const int nameY = tagsSeparatorY - 20;

    const QRect textRect = QRect(x, y + nameY, w, h);

    QTextOption wrapped;
    wrapped.setWrapMode(QTextOption::WordWrap);
    int offset = 0;
    if (hovered ) {
        if (index != m_previousIndex) {
            m_previousIndex = index;
            m_startTime.start();
            m_currentArea = rc;
            m_currentWidget = qobject_cast<QAbstractItemView *>(
                const_cast<QWidget *>(option.widget));
        }
        offset = m_startTime.elapsed() * itemHeight / 200; // Duration 200 ms.
        if (offset < shiftY)
            QTimer::singleShot(5, this, &ImageItemDelegate::goon);
        else if (offset > shiftY)
            offset = shiftY;
    } else {
        m_previousIndex = QModelIndex();
    }

    const QFontMetrics fm(option.widget->font());
    const QRect shiftedTextRect = textRect.adjusted(0, -offset, 0, -offset);

    // The pixmap.
    if (offset == 0) {
        QPixmap pm = itemPixmap;
        QRect inner(x + 11, y - offset, exampleImageSize.width(), exampleImageSize.height());
        QRect pixmapRect = inner;
        if (!pm.isNull()) {
            painter->setPen(foregroundColor2);
            QPoint pixmapPos = pixmapRect.center();
            pixmapPos.rx() -= pm.width() / pm.devicePixelRatio() / 2;
            pixmapPos.ry() -= pm.height() / pm.devicePixelRatio() / 2;
            painter->drawPixmap(pixmapPos, pm);
        } else {
            // The description text as fallback.
            painter->setPen(foregroundColor2);
            painter->setFont(sizedFont(11, option.widget));
            painter->drawText(pixmapRect.adjusted(6, 10, -6, -10), itemDesc, wrapped);
        }
        painter->setPen(foregroundColor1);
        painter->drawRect(pixmapRect.adjusted(-1, -1, -1, -1));
    }

    // The title of the example.
    painter->setPen(foregroundColor1);
    painter->setFont(sizedFont(13, option.widget));
    QRectF nameRect;
    if (offset) {
        nameRect = painter->boundingRect(shiftedTextRect, itemTitle, wrapped);
        painter->drawText(nameRect, itemTitle, wrapped);
    } else {
        nameRect = QRect(x, y + nameY, x + w, y + nameY + 20);
        QString elidedName = fm.elidedText(itemTitle, Qt::ElideRight, w - 20);
        painter->drawText(nameRect, elidedName);
    }

    // The separator line below the example title.
    if (offset) {
        int ll = nameRect.bottom() + 5;
        painter->setPen(lightColor);
        painter->drawLine(x, ll, x + w, ll);
    }

    // The description text.
    if (offset) {
        int dd = nameRect.height() + 10;
        QRect descRect = shiftedTextRect.adjusted(0, dd, 0, dd);
        painter->setPen(foregroundColor2);
        painter->setFont(sizedFont(11, option.widget));
        painter->drawText(descRect, itemDesc, wrapped);
    }

    // Separator line between text and 'Tags:' section
    painter->setPen(lightColor);
    painter->drawLine(x, y + tagsSeparatorY, x + w, y + tagsSeparatorY);

    // The 'Tags:' section
    const int tagsHeight = h - tagsBase;
    const QFont tagsFont = sizedFont(10, option.widget);
    const QFontMetrics tagsFontMetrics(tagsFont);
    QRect tagsLabelRect = QRect(x, y + tagsBase, 30, tagsHeight - 2);
    painter->setPen(foregroundColor2);
    painter->setFont(tagsFont);
    painter->drawText(tagsLabelRect, tr("Tags:"));

    painter->setPen(themeColor());
    m_currentTagRects.clear();
    int xx = 0;
    int yy = y + tagsBase;

    for (const QString tag : itemTags) {
        const int ww = tagsFontMetrics.width(tag) + 5;
        if (xx + ww > w - 30) {
            yy += 15;
            xx = 0;
        }
        const QRect tagRect(xx + x + 30, yy, ww, 15);
        painter->drawText(tagRect, tag);
        m_currentTagRects.append({ tag, tagRect });
        xx += ww;
    }

    // Box it when hovered.
    if (hovered) {
        painter->setPen(lightColor);
        painter->drawRect(rc.adjusted(0, 0, -1, -1));
    }
}

QSize ImageItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(230, 230);
}

ImageItemDelegate::ImageItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}


