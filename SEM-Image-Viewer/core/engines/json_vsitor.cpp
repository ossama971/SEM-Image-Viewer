#include "json_vsitor.h"

#include "../utils.h"
#include "../data/image_format.h"
#include "../data/image_state.h"

#include "../engines/thread_pool.h"
#include "../engines/logger.h"

// TODO: need to handle errors & exceptions that can be thrown by any of the
// visit() methods report the errors through the Logger

JsonVisitor::JsonVisitor(std::string session_datapath, std::string json_filepath, int progressbarID)
    : session_datapath(session_datapath), json_filepath(json_filepath), progressbarID(progressbarID) {}

void JsonVisitor::visit(const ImageMetadata &metadata) {
  boost::property_tree::ptree metadata_tree;
  metadata_tree.put("width", metadata.getWidth());
  metadata_tree.put("height", metadata.getHeight());
  metadata_tree.put("format", imageFormatToString(metadata.getFormat()));
  //metadata_tree.put("colorSpace", colorSpaceToString(metadata.getColorSpace()));
  // metadata_tree.put("dateModified", metadata.getDateModified());
  json_tree.add_child("metadata", metadata_tree);
}

void JsonVisitor::visit(const ImageState &state, Image *image) {
  boost::property_tree::ptree state_tree;
  // TODO: handle the case when session_datapath is empty
  Utils::createDirectory(this->session_datapath);
  const std::string image_filepath = Utils::generateString(11) + state.ImageExtension;
  // TODO: prepend the image_name with the applied filter name before this random string
  const std::filesystem::path image_path(this->session_datapath + "/" + image_filepath);
  if (image && image->save(image_path.string(), (ImageState*)&state)) {
    state_tree.put("state", imageStateSourceToString(state.State));
    state_tree.put("image", image_path.string());
    json_tree.add_child("state", state_tree);
  }
  else {
    state_tree.put("state", "Error");
    state_tree.put("image", "Error");
    json_tree.add_child("state", state_tree);
  }
}

void JsonVisitor::visit(const Image &image) {
  boost::property_tree::ptree image_tree;
  image_tree.put("_loaded", image.isLoaded());
  image_tree.put("_path", image.getPath().string());

  JsonVisitor metadataVisitor(session_datapath, json_filepath, progressbarID);
  image.getMetadata().accept(metadataVisitor);
  image_tree.add_child("metadata", metadataVisitor.json_tree.get_child("metadata"));

  // TODO: saving the states/images to disk should be done concurrently
  // an idea is to have a threadpool and put the saving tasks (states & undos)
  // in a queue and have the threads in the pool take tasks from the queue

  boost::property_tree::ptree states_tree;
  for (const auto &state : image.getStates()) {
    JsonVisitor stateVisitor(session_datapath, json_filepath, progressbarID);
    state->accept(stateVisitor, (Image*)&image);
    states_tree.push_back(std::make_pair("", stateVisitor.json_tree.get_child("state")));
  }
  image_tree.add_child("states", states_tree);

  boost::property_tree::ptree undo_tree;
  for (const auto &undoState : image.getUndo()) {
    JsonVisitor undoVisitor(session_datapath, json_filepath, progressbarID);
    undoState->accept(undoVisitor);
    undo_tree.push_back(std::make_pair("", undoVisitor.json_tree.get_child("state")));
  }
  image_tree.add_child("undo", undo_tree);
  json_tree.add_child("Image", image_tree);
}

void JsonVisitor::visit(const ImageRepository &repo) {
  boost::property_tree::ptree repo_tree;
  repo_tree.put("_folderPath", repo.getFolderPath());

  const std::size_t batch_size = 17;
  std::vector<std::future<boost::property_tree::ptree>> futures;
  const auto& images = repo.getImages();
  const size_t total_images = images.size();
  size_t processed_images = 0;

  for (std::size_t i = 0; i < total_images; i += batch_size) {
    auto start = images.begin() + i;
    auto end = (i + batch_size < total_images) ? start + batch_size : images.end();
    std::vector<Image*> batch(start, end);

    auto future = post(ThreadPool::instance(),
      use_future([
        progressbarID=this->progressbarID,
        session_datapath=this->session_datapath,
        json_filepath=this->json_filepath,
        batch, total_images,
        &processed_images]() -> boost::property_tree::ptree {
        boost::property_tree::ptree local_images_tree;

        for (const auto &image : batch) {
          JsonVisitor imageVisitor(session_datapath, json_filepath, progressbarID);
          image->accept(imageVisitor);
          local_images_tree.push_back(std::make_pair("", imageVisitor.json_tree.get_child("Image")));

          processed_images++;
          float progress = static_cast<float>(processed_images) / static_cast<float>(total_images);
          Logger::instance()->updateProgressBar(progressbarID, progress);
        }

        return local_images_tree;
      }));
    futures.push_back(std::move(future));
  }

  boost::property_tree::ptree images_tree;
  for (auto& future : futures) {
    auto batch_tree = future.get();
    for (auto& node : batch_tree) {
      images_tree.push_back(std::make_pair("", node.second));
    }
  }

  repo_tree.add_child("Images", images_tree);
  json_tree.add_child("ImageRepository", repo_tree);
}


void JsonVisitor::visit(const SessionData &session) {
  boost::property_tree::ptree session_tree;
  JsonVisitor repoVisitor(session_datapath, json_filepath, progressbarID);
  /// Safety: We are casting away const on `session` to access `getImageRepo()`, 
  /// which is logically const in this context. This method does not modify the 
  /// observable state of `SessionData`, and we ensure that no changes are made 
  /// to `session` within `visit()`.
  const_cast<SessionData&>(session).getImageRepo().accept(repoVisitor);
  session_tree.add_child("ImageRepository", repoVisitor.json_tree.get_child("ImageRepository"));
  json_tree.add_child("SessionData", session_tree);
}

void JsonVisitor::write_json() const {
  if(!this->json_filepath.empty()) {
    boost::property_tree::write_json(this->json_filepath, this->json_tree);
  }
}

