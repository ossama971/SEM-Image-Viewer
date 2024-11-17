#include "BatchFilter.h"
#include <QThread>
#include <thread>

BatchFilter::BatchFilter(bool isolateMainThread, int threadCount)
    : _isolateMainThread(isolateMainThread), _threadCount(threadCount ? threadCount : std::thread::hardware_concurrency())
{}

void BatchFilter::apply(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input) {
    if (_isolateMainThread)
    {
        QThread *thread = QThread::create(&BatchFilter::execute, this, std::move(filter), std::move(input));
        thread->start();
    }

    else
        execute(std::move(filter), std::move(input));
}

void BatchFilter::execute(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input) {
    std::vector<cv::Mat> output;

    int threadCount = _threadCount;
    int operationCount = input.size();
    int operationPerThread;

    if (!operationCount || !threadCount)
        return;

    if (operationCount > threadCount)
        operationPerThread = floor(operationCount / threadCount);
    else if (operationCount < threadCount)
    {
        threadCount = operationCount;
        operationPerThread = 1;
    }
    else
        operationPerThread = 1;

    output.resize(operationCount);

    ImageFilter* filterPtr = filter.get();
    std::vector<std::thread> threads;
    int startIndex = 0;
    int lastIndex;

    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex)
    {
        lastIndex = startIndex + operationPerThread + (threadIndex < (operationCount % threadCount));

        threads.emplace_back([this, &input, &output, filterPtr, threadIndex, startIndex, lastIndex]()
        {
            for (int i = startIndex; i < lastIndex; ++i)
            {
                output[i] = filterPtr->applyFilter(*input[i]);

                QMetaObject::invokeMethod(this, "onImageProcessed", Qt::QueuedConnection, Q_ARG(Image*, input[i]));
            }
        });

        startIndex = lastIndex;
    }

    for (auto thread = threads.begin(); thread != threads.end(); ++thread)
    {
        if (thread->joinable())
            thread->join();
    }

    emit onFinish(std::move(input), std::move(output), filter->getImageSource());
}
