#include "ImageState.h"

ImageState::ImageState() {
}

ImageState::ImageState(ImageStateSource state, cv::Mat image) : State(state), Image(image) {
}
