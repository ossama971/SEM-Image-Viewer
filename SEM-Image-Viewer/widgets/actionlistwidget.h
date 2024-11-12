#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H
#include <QWidget>
#include <QVBoxLayout>

class ActionListWidget : public QWidget {
    Q_OBJECT
public:
    explicit ActionListWidget(QWidget* parent = nullptr);

    // Method to add widgets to the container
    void addWidget(QWidget* widget);

private:
    QVBoxLayout* m_itemsLayout;
};
#endif // ACTIONLISTWIDGET_H
