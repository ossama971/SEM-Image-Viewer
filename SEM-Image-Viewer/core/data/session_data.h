#ifndef IMAGE_SESSION_H
#define IMAGE_SESSION_H

#include "image_repository.h"
#include "Visitor.h"
#include "Visitable.h"
#include "../filters/image_filter.h"
#include "../filters/batch_filter.h"

class SessionData : public QObject, public Visitable {
  Q_OBJECT
public:
    SessionData();

    void loadDirectory(const std::string &path);
    void loadImage(const std::string &path);
    void applyFilter(std::unique_ptr<ImageFilter> filter);
    void applyFilter(std::unique_ptr<ImageFilter> filter, const std::vector<int> &image_indices);

    bool undo();
    bool redo();

    std::vector<int> pixelIntensity(const std::vector<std::pair<int, int>> &points);
    cv::Mat heatMap();
    cv::Mat diffTwoImages(const cv::Mat &image2, const int threshold);

    ImageRepository& getImageRepo();
    Image* getSelectedImage();

    void accept(Visitor &v) const override;

    bool horizontalIntensityPlotMode = false;
    void toggleHorizontalPlotMode();

signals:
    void loadActionList(const QList<QString> &actions);
    void updateActionList(const QString &action);
    void popActionList();

    void onBatchFilterStarted(int maxIterations);
    void onBatchFilterFinished();    


private slots:
    void onBatchFilterApplied(const std::vector<Image*> &input, const std::vector<std::pair<cv::Mat, bool>> &output, ImageStateSource stateSource);

private:
    ImageRepository _imageRepo;
    BatchFilter _batchFilter;
};

#endif // IMAGE_SESSION_H
