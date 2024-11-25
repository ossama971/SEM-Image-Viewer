#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <QWidget>
#include <QProgressBar>

class ProgressBarComponent : public QProgressBar  {
    Q_OBJECT

public:
    explicit ProgressBarComponent(QWidget *parent = nullptr);
public slots:
    void setMaxIterations(int max);
    void incrementProgress();
    void resetProgress();
    void hideProgressBar();

private:
    QProgressBar *progressBar = nullptr;
};
#endif // PROGRESS_BAR_H
