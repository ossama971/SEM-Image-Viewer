#ifndef WRITABLE_TARGET_H
#define WRITABLE_TARGET_H

#include "Buffer.h"

// this mainly acts as a marker interface for any object that can be written
// to any device
//
// TODO: consider using async/await?
class WritableTarget {
public:
  virtual void write(const Buffer &buffer) const = 0;
  virtual ~WritableTarget() = default;
};

#endif // WRITABLE_TARGET_H
