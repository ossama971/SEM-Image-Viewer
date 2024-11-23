#ifndef LOGCARDWIDGET_H
#define LOGCARDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "progressbar.h"
class LogCard : public QWidget
{
    Q_OBJECT

public:

    explicit LogCard(const QString typeOfMessage,const QString &headerText, const QString &detailsText, bool progressBarFlag,const QString & path, QWidget *parent=nullptr);
    ProgressBarComponent * getProgressBar();
    QString getType();
signals:
     void sizeChanged();
private slots:
    void toggleDetails();
    void onCopyContent();
    void onClickOnPath();
private:
    QLabel *headerLabel;
    QLabel *detailsLabel;
    QWidget* detailsContainer;
    QPushButton *detailsBtn;
    QPushButton * copyBtn;
    ProgressBarComponent * progressBar;
    bool detailsVisible;
    QString _type;
    QString _path;
};

#endif
