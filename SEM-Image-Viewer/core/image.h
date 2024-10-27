#include <opencv2/opencv.hpp>

class Image {
private:
  cv::Mat image;
  std::string path;

public:
  Image(const std::string &path);
  ~Image();

  cv::Mat getImage() const;
  std::string getPath() const;

  int getWidth() const;
  int getHeight() const;
  int getSize() const;
};
