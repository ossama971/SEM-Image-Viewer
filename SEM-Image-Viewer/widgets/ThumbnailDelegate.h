#ifndef THUMBNAILDELEGATE_H
#define THUMBNAILDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class ThumbnailDelegate : public QStyledItemDelegate {
public:
    ThumbnailDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        // Let the base class handle the thumbnail rendering
        QStyledItemDelegate::paint(painter, option, index);

        // Check if the current item is selected
        if (option.state & QStyle::State_Selected) {
            // Set up a blue pen for the border
            QPen borderPen(Qt::white);
            painter->setPen(borderPen);
            painter->setBrush(Qt::NoBrush); // No fill, only border

            // Draw the border rectangle slightly inside the option's rect
            QRect borderRect = option.rect.adjusted(2, 2, -2, -2); // Add padding for aesthetics
            painter->drawRect(borderRect);
        }
    }
};

#endif // THUMBNAILDELEGATE_H
