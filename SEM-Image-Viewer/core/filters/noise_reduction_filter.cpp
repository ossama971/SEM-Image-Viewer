#include "noise_reduction_filter.h"
#include "../engines/logger.h"

NoiseReductionFilter::NoiseReductionFilter(double intensity)
{
    if (intensity < 0)
        _intensity = 0;
    else if (intensity > 100)
        _intensity = 100;
    else
        _intensity = intensity;
}

bool NoiseReductionFilter::applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log) const
{
    const cv::Mat& image = inputImage.readImageMat();

    if (image.channels() == 1 || inputImage.getMetadata().getColorSpace(image) == ColorSpace::Gray)
    {
        cv::fastNlMeansDenoising(cv::InputArray(image), cv::OutputArray(outputImage), _intensity);
    }
    else
    {
        cv::fastNlMeansDenoisingColored(cv::InputArray(image), cv::OutputArray(outputImage), _intensity, _intensity);
    }

    if (log)
    {
        Logger::instance()->logMessage(
            Logger::MessageTypes::info, Logger::MessageID::filter_applied,
            Logger::MessageOption::with_path,
            {"Noise Reduction"},
            "https://docs.opencv.org/4.x/d5/d69/tutorial_py_non_local_means.html");
    }
    return true;
}

ImageStateSource NoiseReductionFilter::getImageSource() const
{
    return ImageStateSource::NoiseReductionFilter;
}

const double NoiseReductionFilter::getIntensity(void) const
{
    return _intensity;
}
