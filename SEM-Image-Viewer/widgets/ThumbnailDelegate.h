#ifndef THUMBNAILDELEGATE_H
#define THUMBNAILDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class ThumbnailDelegate : public QStyledItemDelegate {
public:
    ThumbnailDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent), padding(7), highlightedPadding(3), cornerRadius(5) {}

    void setPadding(int p) {
        padding = p;
    }

    void setHighlightedPadding(int p) {
        highlightedPadding = p;  // Set how much padding to highlight
    }

    void setCornerRadius(int radius) {
        cornerRadius = radius;
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QStyleOptionViewItem paddedOption = option;

        // Adjust the rectangle to include padding
        paddedOption.rect.adjust(padding, padding, -padding, -padding);

        painter->save();

        // Highlight the padding area if the item is selected
        if (option.state & QStyle::State_Selected) {
            // Calculate the inner padding area to highlight (only highlightedPadding of the total padding)
            QRect paddingArea = option.rect;
            int highlightPadding = highlightedPadding;

            // Adjust the padding area to highlight only a part of the padding
            paddingArea.adjust(highlightPadding, highlightPadding, -highlightPadding, -highlightPadding);

            QColor highlightColor(120, 120, 120, 100);  // Light transparent color
            painter->setBrush(highlightColor);  // Set the highlight color
            painter->setPen(Qt::NoPen);         // No border
            painter->drawRoundedRect(paddingArea, cornerRadius, cornerRadius);  // Draw the rounded highlight
        }

        // Retrieve the pixmap from the model
        QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();

        if (!pixmap.isNull()) {
            // Calculate the target rect for the image
            QRect targetRect = paddedOption.rect;

            // Scale the image proportionally
            QSize imageSize = pixmap.size();
            imageSize.scale(targetRect.size(), Qt::KeepAspectRatio);

            QRect centeredRect(
                targetRect.left() + (targetRect.width() - imageSize.width()) / 2,
                targetRect.top() + (targetRect.height() - imageSize.height()) / 2,
                imageSize.width(),
                imageSize.height()
                );

            // Draw the pixmap centered in the target rect
            painter->drawPixmap(centeredRect, pixmap);
        }

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QSize originalSize = QStyledItemDelegate::sizeHint(option, index);
        // Add padding to the size
        return QSize(originalSize.width() + padding * 2, originalSize.height() + padding * 2);
    }

private:
    int padding;             // Total padding around the image
    int highlightedPadding;  // Padding to be highlighted
    int cornerRadius;        // Radius for rounded corners
};

#endif // THUMBNAILDELEGATE_H
