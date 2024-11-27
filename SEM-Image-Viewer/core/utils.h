#include <opencv2/opencv.hpp>
#include <filesystem>
#include <QImage>

class Image;

namespace Utils {
    std::string generateString(size_t length);
    cv::Mat diffTwoImages(const cv::Mat &image1, const cv::Mat &image2, const int threshold);
    bool checkWritePermission(const std::filesystem::path &folderPath);
    bool createDirectory(const std::string &path);
    void loadSessionJson(const std::string &filename);
    cv::Mat heatmap(const cv::Mat& image);
    QImage loadFromQrc(const QString &qrc, const char *extension);
    cv::Mat loadFromQrc(const QString &qrc, int flag = cv::IMREAD_COLOR);
    cv::Mat imageToMat(const QImage &image);
    QImage matToImage(const cv::Mat &image);
};
