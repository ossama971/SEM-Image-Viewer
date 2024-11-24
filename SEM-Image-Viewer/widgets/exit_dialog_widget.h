#ifndef ExitDialogWidget_H
#define ExitDialogWidget_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ExitDialogWidget : public QDialog {
    Q_OBJECT
public:
    explicit ExitDialogWidget(QWidget *parent = nullptr);

signals:
    void saveRequested();
    void dontSaveRequested();

private slots:
    void onSaveClicked();
    void onCancelClicked();
    void onDontSaveClicked();

};

#endif // ExitDialogWidget_H
