#include "batch_filter.h"
#include "../engines/logger.h"
#include "../engines/thread_pool.h"

void BatchFilter::apply(std::unique_ptr<ImageFilter> filter, const std::vector<Image*> &input) {
  if (input.empty() || !filter)
    return;

  startTime = QDateTime::currentDateTime();
  
  int progressbarID = Logger::instance()->logMessageWithProgressBar(
      Logger::MessageTypes::INFO,
      Logger::MessageID::BATCH_FILTER_APPLIED,
      Logger::MessageOptian::WITHOUT_DETIALS,
      {  },
      input.size(),
      ""
  );

  post(ThreadPool::instance(), [this, filter = std::move(filter), input = std::move(input), progressbarID]() {

    std::vector<cv::Mat> output(input.size());
    ImageFilter* filterPtr = filter.get();

    const std::size_t batch_size = 17;

    std::vector<std::future<void>> futures;
    size_t processed_images = 0;

    for (std::size_t i = 0; i < input.size(); i += batch_size) {
      auto start = input.begin() + i;
      auto end = (i + batch_size < input.size()) ? start + batch_size : input.end();
      std::vector<Image*> batch(start, end);

      auto future = post(ThreadPool::instance(),
          use_future([filterPtr, batch, progressbarID, &output, &processed_images]() {
            for (std::size_t i = 0; i < batch.size(); ++i) {
              output[i] = filterPtr->applyFilter(*batch[i]);
              processed_images++;
              float progress = static_cast<float>(processed_images) / static_cast<float>(output.size());
              Logger::instance()->updateProgressBar(progressbarID, progress);
            }
          }));
      futures.push_back(std::move(future));
    }

    for (auto& future : futures) {
      future.get();
    }

    emit onFinish(std::move(input), std::move(output), filter->getImageSource());
  });
}
