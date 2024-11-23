#ifndef OTHERLISTWIDGET_H
#define OTHERLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
class OtherListWidget : public QWidget {
    Q_OBJECT
public:
    explicit OtherListWidget(QWidget* parent = nullptr);

    // Method to add widgets to the container
    void addWidget(QWidget* widget);

private:
    QVBoxLayout* m_itemsLayout;
};

#endif // OTHERLISTWIDGET_H
