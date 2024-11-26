#ifndef VISITOR_H
#define VISITOR_H

class ImageMetadata;
class ImageState;
class Image;
class ImageRepository;
class SessionData;

class Visitor {
public:
  virtual void visit(const ImageMetadata &metadata) = 0;
  virtual void visit(const ImageState &state, Image *image) = 0;
  virtual void visit(const Image &image) = 0;
  virtual void visit(const ImageRepository &repo) = 0;
  virtual void visit(const SessionData &session) = 0;

  virtual ~Visitor() = default;
};

#endif // VISITOR_H
