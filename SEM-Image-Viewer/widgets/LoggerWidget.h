#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

#include "../models/MessageDataModel.h"
#include "../core/engines/Logger.h"

class LoggerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr, std::shared_ptr<MessageDataModel> dataModel = nullptr);

private slots:
    void filterLogs();
    void switchLayout();
    void createConnections();
    void createLayouts();
    void createButtons();

private:
    QPushButton *allShowButton;
    QPushButton *infoShowButton;
    QPushButton *errorsShowButton;
    QPushButton *warningsShowButton;

    QPushButton *switchLayoutButtonFull;
    QPushButton *switchLayoutButtonCompact;
    QVBoxLayout *fullLayout;
    QVBoxLayout *compactLayout;
    QStackedWidget *stackedWidget;
    bool isExpanded;

    QLineEdit *searchLineEdit;
    QListView *logListWidget;

    std::shared_ptr<MessageDataModel> m_dataModel;
};

#endif // LOGGERWIDGET_H
