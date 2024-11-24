#ifndef JSON_VISITOR_H
#define JSON_VISITOR_H

#include "../data/Visitor.h"
#include "../data/image_meta_data.h"
#include "../data/image_state.h"
#include "../data/image.h"
#include "../data/image_repository.h"
#include "../data/session_data.h"

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
  JsonVisitor(std::string session_datapath, std::string json_filepath, int progressbarID);
  ~JsonVisitor() override = default;

private:
    boost::property_tree::ptree json_tree;
    std::string session_datapath;
    std::string json_filepath;
    // TODO: the visitor should not be aware of the progressbar. there should be
    // a better way to notify the progressbar as a seperate entity about the
    // progress of the visitor
    int progressbarID;
};

#endif // JSON_VISITOR_H
