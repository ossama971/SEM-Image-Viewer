#include <opencv2/opencv.hpp>
#include <QString>
#include <QImage>

class Image;

namespace Utils {
    std::string generateString(size_t length);
    cv::Mat diffTwoImages(const cv::Mat &image1, const cv::Mat &image2, const int threshold);
    bool createDirectory(const std::string &path);
    void loadSessionJson(const std::string &filename);
    std::optional<std::pair<QImage, QString>> prepareImageForExport(const Image *image, const QString &directoryPath, const QString &format);

};
