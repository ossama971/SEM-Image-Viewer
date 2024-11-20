#ifndef JSON_VISITOR_H
#define JSON_VISITOR_H

#include "../data/Visitor.h"
#include "../data/ImageMetadata.h"
#include "../data/ImageState.h"
#include "../data/Image.h"
#include "../data/ImageRepository.h"
#include "../data/SessionData.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class JsonVisitor : public Visitor {
public:
  void visit(const ImageMetadata &metadata) override;
  void visit(const ImageState &state) override;
  void visit(const Image &image) override;
  void visit(const ImageRepository &repo) override;
  void visit(const SessionData &session) override;

  void write_json() const;

  JsonVisitor() = default;
  JsonVisitor(std::string session_datapath, std::string json_filepath);
  ~JsonVisitor() override = default;

  // TODO: should have a better way to enforce setting these paths
  void set_session_datapath(const std::filesystem::path& path);
  void set_json_filepath(const std::filesystem::path& path);

private:
    boost::property_tree::ptree json_tree;
    std::string session_datapath;
    std::string json_filepath;
};

#endif // JSON_VISITOR_H
