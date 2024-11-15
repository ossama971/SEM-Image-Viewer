#ifndef SAVEDIALOGWIDGET_H
#define SAVEDIALOGWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SaveDialogWidget : public QDialog {
    Q_OBJECT
public:
    explicit SaveDialogWidget(QWidget *parent = nullptr);

signals:
    void saveRequested();
    void dontSaveRequested();

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void onDontSaveClicked();

};

#endif // SAVEDIALOGWIDGET_H
