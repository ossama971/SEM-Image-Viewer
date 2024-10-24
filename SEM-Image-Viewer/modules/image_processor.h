#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QPixmap>
#include <optional>
#include <string>

class ImageProcessor
{
public:
    ImageProcessor();  // Constructor
    std::optional<QPixmap> loadAndPrepareImage(const std::string &imagePath, const QSize &size);  // Load and prepare image for display
};

#endif // IMAGEPROCESSOR_H
