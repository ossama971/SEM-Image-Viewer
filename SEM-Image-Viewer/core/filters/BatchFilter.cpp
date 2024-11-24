#include "BatchFilter.h"
#include "../engines/Logger.h"
#include "../engines/ThreadPool.h"

void BatchFilter::apply(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input) {
  if (input.empty() || !filter)
    return;
  startTime = QDateTime::currentDateTime();
  // Logger::instance()->log(std::make_unique<InfoMessage>(LOG_INFO,boost::format(LogMessageMapper::filterStarted().toStdString())));
  std::vector<cv::Mat> output(input.size());
  ImageFilter* filterPtr = filter.get();

  const std::size_t batch_size = 17;

  std::vector<std::future<void>> futures;
  for (std::size_t i = 0; i < input.size(); i += batch_size) {
    auto start = input.begin() + i;
    auto end = (i + batch_size < input.size()) ? start + batch_size : input.end();
    std::vector<Image*> batch(start, end);

    auto future = post(ThreadPool::instance(),
        use_future([this, filterPtr, batch, &output]() {
          for (std::size_t i = 0; i < batch.size(); ++i) {
            output[i] = filterPtr->applyFilter(*batch[i]);
            emit onImageProcessed(batch[i]);
          }
        }));
    futures.push_back(std::move(future));
  }

  for (auto& future : futures) {
    future.get();
  }

  //  auto duration = startTime.msecsTo(QDateTime::currentDateTime());
  //  Logger::instance()->log(std::make_unique<InfoMessage>(LOG_INFO,boost::format(LogMessageMapper::filterCompleted(duration).toStdString())));

  emit onFinish(std::move(input), std::move(output), filter->getImageSource());
}

