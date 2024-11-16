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

  void write_json(const std::string& filename) const;

  ~JsonVisitor() override = default;

private:
    boost::property_tree::ptree json_tree;
};

#endif // JSON_VISITOR_H
