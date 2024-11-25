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

    explicit LogCard(const QString &typeOfMessage,const QString &headerText, const QString &detailsText, bool progressBarFlag,const QString & path, QWidget *parent=nullptr);
    ProgressBarComponent * getProgressBar();
    QString getType();
signals:
     void sizeChanged();
private slots:
    void toggleDetails();
    void onCopyContent();
    void onClickOnPath();
private:
    QLabel *headerLabel = nullptr;
    QLabel *detailsLabel = nullptr;
    QWidget* detailsContainer = nullptr;
    QPushButton *detailsBtn = nullptr;
    QPushButton * copyBtn = nullptr;
    ProgressBarComponent * progressBar = nullptr;
    bool detailsVisible = false;
    QString _type;
    QString _path;
};

#endif
