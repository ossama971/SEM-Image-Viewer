#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <QWidget>
#include <QProgressBar>
#include <QVBoxLayout>

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
    QProgressBar *progressBar;
};
#endif // PROGRESS_BAR_H
