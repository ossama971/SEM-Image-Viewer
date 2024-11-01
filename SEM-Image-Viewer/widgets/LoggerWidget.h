#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>

class LoggerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggerWidget(QWidget *parent = nullptr);
    void addLogMessage(const QString &type, const QString &message);

private slots:
    void filterLogs();
    void switchLayout();

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
    QListWidget *logListWidget;
};

#endif // LOGGERWIDGET_H
