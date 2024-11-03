#include <boost/algorithm/string.hpp>
#include <boost/atomic.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include "Filter.h"
#include "Level.h"
#include "Log.h"
#include "Logger.h"
#include "Metadata.h"
#include "NopLogger.h"
#include "Record.h"
#include "Target.h"
#include "Writer.h"

constexpr uint8_t UNINTIALIZED = 0;
constexpr uint8_t INTIALIZING = 1;
constexpr uint8_t INTIALIZED = 2;

// XXX: rethink these globals, for thread safety
boost::atomic<uint8_t> MAX_LOG_LEVEL_FILTER{0};

Log *LOGGER = NopLogger::get_instance();

boost::atomic<uint8_t> STATE{UNINTIALIZED};

void set_max_level(Level level) {
  MAX_LOG_LEVEL_FILTER.store(static_cast<uint8_t>(level.get_typ()),
                             boost::memory_order_relaxed);
}

Level get_max_level() {
  return Level(static_cast<Level::LevelType>(
      MAX_LOG_LEVEL_FILTER.load(boost::memory_order_relaxed)));
}

std::pair<bool, const char *> set_logger(boost::function<Log *()> make_logger) {
  uint8_t expected = UNINTIALIZED;
  if (STATE.compare_exchange_strong(expected, INTIALIZING,
                                    boost::memory_order_acquire,
                                    boost::memory_order_relaxed)) {
    LOGGER = make_logger();
    STATE.store(INTIALIZED, boost::memory_order_release);
    return std::make_pair(true, "Logger set successfully");

  } else if (expected == INTIALIZING) {
    while (STATE.load(boost::memory_order_relaxed) == INTIALIZING) {
      // on using yield instead of spinlock
      // https://doc.rust-lang.org/std/hint/fn.spin_loop.html
      // https://github.com/facebook/folly/blob/208949c27d3c7e6f6136d64de3108a6c71fae95b/folly/SpinLock.h#L17
      boost::this_thread::yield();
    }
    return std::make_pair(false, SET_LOGGER_ERROR);

  } else {
    return std::make_pair(false, SET_LOGGER_ERROR);
  }
}

Log *get_logger() {
  if (STATE.load(boost::memory_order_relaxed) != INTIALIZED) {
    return NopLogger::get_instance();
  } else {
    return LOGGER;
  }
}

class StringTarget : public WritableTarget {
public:
  StringTarget() : buffer_() {}

  void write(const Buffer &buffer) const override {
    buffer_.append(reinterpret_cast<const char *>(buffer.as_bytes().data()),
                   buffer.as_bytes().size());
  }

  std::string get_value() { return buffer_; }

private:
  mutable std::string buffer_;
};


int32_t main(void) {

  boost::shared_ptr<WritableTarget> target(new StringTarget());
  BufferWriter buffer_writer(target);

  // std::string value = "Hello World";
  // Buffer buffer;
  // std::vector<uint8_t> bytes(value.size() + 1);
  // bytes.insert(bytes.end(), value.begin(), value.end());
  // buffer.write(bytes);
  // buffer_writer.write(buffer);

  Writer writer(buffer_writer);
  FilterOp filter_op("[hH]ello");
  Filter filter = FilterBuilder()
                      .add_directive(Directive("main", Level_INFO))
                      .add_directive(Directive("details", Level_TRACE))
                      .set_filter(filter_op)
                      .build();

  // writer.print(buffer);
  //
  LoggerBuilder builder;
  builder.set_writer(writer).set_filter(filter).build();

  //
  set_logger(Logger::get_instance);
  // set_max_level(Level_TRACE);

  Record r1 = Record::builder()
                  .set_metadata(Metadata::builder()
                                    .set_level(Level_INFO)
                                    .set_target("main")
                                    .build())
                  .set_args(boost::format("Hello %1%") % "MAIN")
                  .set_module_path(__FUNCTION__)
                  .set_file(__FILE__)
                  .set_line(__LINE__)
                  .build();

  Record r2 = Record::builder()
                  .set_metadata(Metadata::builder()
                                    .set_level(Level_INFO)
                                    .set_target("details")
                                    .build())
                  .set_args(boost::format("Hello %1%") % "DETAILS")
                  .set_module_path(__FUNCTION__)
                  .set_file(__FILE__)
                  .set_line(__LINE__)
                  .build();

  Record r3 = Record::builder()
                  .set_metadata(Metadata::builder()
                                    .set_level(Level_DEBUG)
                                    .set_target("main")
                                    .build())
                  .set_args(boost::format("Hello %1%") % "MAIN")
                  .set_module_path(__FUNCTION__)
                  .set_file(__FILE__)
                  .set_line(__LINE__)
                  .build();

  get_logger()->log(r1);
  get_logger()->log(r2);
  get_logger()->log(r3);

  std::cout << static_cast<StringTarget *>(target.get())->get_value();

  delete NopLogger::get_instance();
  delete Logger::get_instance();
  return 0;
}
