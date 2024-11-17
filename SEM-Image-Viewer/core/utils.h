#include <opencv2/opencv.hpp>

namespace Utils {
    std::string generateString(size_t length);
    cv::Mat diffTwoImages(const cv::Mat &image1, const cv::Mat &image2, const int threshold);
    void loadSessionJson(const std::string &filename);
};
