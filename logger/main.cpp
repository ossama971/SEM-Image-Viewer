#include <boost/algorithm/string.hpp>
#include <boost/atomic.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include "src/LoggerUtils.h"
#include "src/filter/Filter.h"
#include "src/Level.h"
#include "src/Log.h"
#include "src/Logger.h"
#include "src/Metadata.h"
#include "src/NopLogger.h"
#include "src/Record.h"
#include "src/writer/Writer.h"

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
