#ifndef THUMBNAILDELEGATE_H
#define THUMBNAILDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>

class ThumbnailDelegate : public QStyledItemDelegate {
public:
    ThumbnailDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyledItemDelegate::paint(painter, option, index);

        if (option.state & QStyle::State_Selected) {
            // Draw a border around the selected item
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(option.rect);
        }
    }
};

#endif // THUMBNAILDELEGATE_H
