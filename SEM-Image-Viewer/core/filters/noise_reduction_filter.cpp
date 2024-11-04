#include "noise_reduction_filter.h"
#include "../image_utils.h"

NoiseReductionFilter::NoiseReductionFilter(double intensity) {
    if (intensity < 0)
        _intensity = 0;
    else if (intensity > 100)
        _intensity = 100;
    else
        _intensity = intensity;
}

cv::Mat NoiseReductionFilter::applyFilter(const cv::Mat& inputImage) const {
    cv::Mat res;

    if (ImageUtils::isGreyScale(inputImage))
    {
        cv::fastNlMeansDenoising(cv::InputArray(inputImage), cv::OutputArray(res), _intensity);
    }
    else
    {
        cv::fastNlMeansDenoisingColored(cv::InputArray(inputImage), cv::OutputArray(res), _intensity, _intensity);
    }
    return res;
}
