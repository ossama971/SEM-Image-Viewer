#ifndef VISITABLE_H
#define VISITABLE_H

class Visitor;

class Visitable {
public:
    virtual void accept(Visitor &v) const = 0;
    virtual ~Visitable() = default;
};

#endif // VISITABLE_H
