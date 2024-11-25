#ifndef LOG_CARD_WIDGET_H
#define LOG_CARD_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "progress_bar.h"
class LogCard : public QWidget
{
    Q_OBJECT

public:

    explicit LogCard(const QString &typeOfMessage,const QString &headerText, bool progressBarFlag,bool colorFlag,const QString & path, QWidget *parent=nullptr);
    ProgressBarComponent * getProgressBar();
    QString getType();
signals:
     void sizeChanged();
private slots:
    void onCopyContent();
    void onClickOnPath();
private:
    QLabel *headerLabel = nullptr;
    QPushButton * copyBtn = nullptr;
     QPushButton * pathBtn = nullptr;
    ProgressBarComponent * progressBar = nullptr;
    QString _type;
    QString _path;
};

#endif
