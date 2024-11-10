#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>

class ProgressBarComponent : public QWidget {
    Q_OBJECT

public:
    ProgressBarComponent(QWidget *parent = nullptr);

public slots:
    void setMaxIterations(int max);
    void incrementProgress();
    void resetProgress();
    void hideProgressBar();

private:
    QProgressBar *progressBar;
};
#endif // PROGRESSBAR_H
