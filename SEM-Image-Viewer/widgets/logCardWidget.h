#ifndef LOGCARDWIDGET_H
#define LOGCARDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class LogCard : public QWidget
{
    Q_OBJECT

public:
    explicit LogCard(const QString &headerText, const QString &detailsText, QWidget *parent = nullptr);

private slots:
    void toggleDetails();

private:
    QLabel *headerLabel;
    QLabel *detailsLabel;
    QPushButton *detailsBtn;
    bool detailsVisible;
};

#endif
