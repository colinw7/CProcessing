#ifndef PObject_H
#define PObject_H

class PObject {
 public:
  PObject() :
   count_(0) {
  }

  PObject(const PObject &o) :
   count_(o.count_) {
  }

  PObject &operator=(const PObject &o) {
    count_ = o.count_;

    return *this;
  }

  virtual ~PObject() { }

  void addRef   () { ++count_; }
  void removeRef() { --count_; }

  int getRef() const { return count_; }

 private:
  int count_;
};

typedef IRefPtr<PObject> PObjectP;

extern PObjectP null;

#endif
