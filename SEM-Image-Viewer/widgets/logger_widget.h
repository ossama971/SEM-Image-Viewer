#ifndef LOGGER_WIDGET_H
#define LOGGER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include "../core/engines/logger.h"
#include "log_card_widget.h"

class LoggerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);

    void addLogCard(LogCard *card);
    void clearLogCards();
private slots:
    void filterLogs();
    void switchLayout();
    void createConnections();
    void createLayouts();
    void createButtons();

signals:
    void showSelectedType(const QString &type, const QString &_searchText);

private:
    QPushButton *allShowButton = nullptr;
    QPushButton *infoShowButton = nullptr;
    QPushButton *errorsShowButton = nullptr;
    QPushButton *warningsShowButton = nullptr;
    QPushButton *switchLayoutButtonFull = nullptr;
    QPushButton *switchLayoutButtonCompact = nullptr;

    QVBoxLayout *fullLayout = nullptr;
    QVBoxLayout *compactLayout = nullptr;
    QHBoxLayout *layout = nullptr;

    QStackedWidget *stackedWidget = nullptr;
    QLineEdit *searchLineEdit = nullptr;
    QListWidget *logListLayout = nullptr;

    bool isExpanded = true;
    int errorCount=0;
    int infoCount=0;
    int warningCount=0;
signals:
    void layoutSwitched(bool isExpanded);
};

#endif // LOGGER_WIDGET_H
