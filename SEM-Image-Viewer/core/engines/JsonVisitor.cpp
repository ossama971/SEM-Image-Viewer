#include "JsonVisitor.h"

#include "../data/ImageFormat.h"
#include "../data/ImageColorSpace.h"
#include "../data/ImageState.h"

void JsonVisitor::visit(const ImageMetadata &metadata) {
  boost::property_tree::ptree metadata_tree;
  metadata_tree.put("width", metadata.getWidth());
  metadata_tree.put("height", metadata.getHeight());
  metadata_tree.put("format", imageFormatToString(metadata.getFormat()));
  metadata_tree.put("colorSpace", colorSpaceToString(metadata.getColorSpace()));
  // metadata_tree.put("dateModified", metadata.getDateModified());
  json_tree.add_child("metadata", metadata_tree);
}

void JsonVisitor::visit(const ImageState &state) {
  boost::property_tree::ptree state_tree;
  state_tree.put("state", imageStateSourceToString(state.State));
  state_tree.put("image", state.getImageBase64());
  json_tree.add_child("state", state_tree);
}

void JsonVisitor::visit(const Image &image) {
  boost::property_tree::ptree image_tree;
  image_tree.put("_loaded", image.isLoaded());
  image_tree.put("_path", image.getPath().string());

  JsonVisitor metadataVisitor;
  image.getMetadata().accept(metadataVisitor);
  image_tree.add_child("metadata", metadataVisitor.json_tree.get_child("metadata"));

  boost::property_tree::ptree states_tree;
  for (const auto &state : image.getStates()) {
    JsonVisitor stateVisitor;
    state->accept(stateVisitor);
    states_tree.push_back(std::make_pair("", stateVisitor.json_tree.get_child("state")));
  }
  image_tree.add_child("states", states_tree);

  boost::property_tree::ptree undo_tree;
  for (const auto &undoState : image.getUndo()) {
    JsonVisitor undoVisitor;
    undoState->accept(undoVisitor);
    undo_tree.push_back(std::make_pair("", undoVisitor.json_tree.get_child("state")));
  }
  image_tree.add_child("undo", undo_tree);
  json_tree.add_child("Image", image_tree);
}

void JsonVisitor::visit(const ImageRepository &repo) {
  boost::property_tree::ptree repo_tree;
  repo_tree.put("_folderPath", repo.getFolderPath());

  boost::property_tree::ptree images_tree;
  for (const auto &image : repo.getImages()) {
    JsonVisitor imageVisitor;
    image.accept(imageVisitor);
    images_tree.push_back(std::make_pair("", imageVisitor.json_tree.get_child("Image")));
  }
  repo_tree.add_child("Images", images_tree);
  json_tree.add_child("ImageRepository", repo_tree);
}

void JsonVisitor::visit(const SessionData &session) {
  boost::property_tree::ptree session_tree;
  JsonVisitor repoVisitor;
  /// Safety: We are casting away const on `session` to access `getImageRepo()`, 
  /// which is logically const in this context. This method does not modify the 
  /// observable state of `SessionData`, and we ensure that no changes are made 
  /// to `session` within `visit()`.
  const_cast<SessionData&>(session).getImageRepo().accept(repoVisitor);
  session_tree.add_child("ImageRepository", repoVisitor.json_tree.get_child("ImageRepository"));
  json_tree.add_child("SessionData", session_tree);
}

void JsonVisitor::write_json(const std::string &filename) const {
  // TODO: handle errors & log errors if can't write to file
  boost::property_tree::write_json(filename, json_tree);
}

