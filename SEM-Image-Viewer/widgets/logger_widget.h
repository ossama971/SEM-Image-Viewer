#ifndef LOGGER_WIDGET_H
#define LOGGER_WIDGET_H

#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QListWidget>

#include <QListWidgetItem>

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
    void showSelectedType(QString type, QString _searchText);

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
    QHBoxLayout *layout;

    QListWidget *logListLayout;



signals:
    void layoutSwitched(bool isExpanded);
};

#endif // LOGGER_WIDGET_H
