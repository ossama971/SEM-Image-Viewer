#include <vector>
#include <opencv2/opencv.hpp>

class ImageUtils {
  public:
    static cv::Mat getImageProperties(const std::string &path);
    static std::vector<int> pixelIntensity(const cv::Mat &image, const std::vector<std::pair<int, int>> &points);
    static cv::Mat heatMap(const cv::Mat &image);
    static cv::Mat diffTwoImages(const cv::Mat &image1, const cv::Mat &image2, const int threshold);
};
