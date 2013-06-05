#ifndef IRefPtr_H
#define IRefPtr_H

template<typename T, typename AT=int&>
class IRefPtr {
 protected:
  template<typename S, typename AS>
  friend class IRefPtr;

 public:
  IRefPtr(T *p=NULL) :
   p_(p) {
    addRef();
  }

  IRefPtr(const IRefPtr &p) :
   p_(p.p_) {
    addRef();
  }

  IRefPtr &operator=(const IRefPtr &p) {
    if (p.p_ == p_) return *this;

    p.addRef();

    removeRef();

    p_ = p.p_;

    return *this;
  }

 ~IRefPtr() {
    removeRef();
  }

  // p->
  T *operator->() {
    return getPtr();
  }

  // p->
  T *operator->() const {
    return getPtr();
  }

  // *p
  T &operator*() const {
    return *getPtr();
  }

  // conversion
  template<typename S>
  operator IRefPtr<S,AT>() const {
    return IRefPtr<S,AT>((S *) p_);
  }

  // subscript
  AT operator[](int i) {
    return (*getPtr())[i];
  }

  T *getPtr() const {
    assert(p_ != 0 && getRef() > 0);

    return p_;
  }

  template<typename S>
  IRefPtr &operator=(const IRefPtr<S> &p) {
    if (p.p_ == p_) return *this;

    p.addRef();

    removeRef();

    p_ = (T *) p.p_;

    return *this;
  }

  IRefPtr &operator=(const T *p) {
    if (p == p_) return *this;

    p->addRef();

    removeRef();

    p_ = const_cast<T *>(p);

    return *this;
  }

  IRefPtr &operator=(T *p) {
    if (p == p_)
      return *this;

    p->addRef();

    removeRef();

    p_ = p;

    return *this;
  }

  bool operator==(const IRefPtr &p) const {
    return p_ == p.p_;
  }

  bool operator!=(const IRefPtr &p) const {
    return p_ != p.p_;
  }

  bool operator==(const T *p) const {
    return p_ == p;
  }

  bool operator!=(const T *p) const {
    return p_ != p;
  }

  bool operator<(const T *p) const {
    return p_ < p;
  }

  // if (p)
  operator bool() const {
    return isValid();
  }

  bool isValid() const {
    return (p_ != 0);
  }

  int getRef() const {
    if (p_)
      return p_->getRef();
    else
      return 1;
  }

  template<typename S>
  S *
  cast() const {
    return dynamic_cast<S *>(p_);
  }

  template<typename S>
  bool
  canCast() const {
    return (dynamic_cast<S *>(p_) != NULL);
  }

  template<typename S>
  IRefPtr<S>
  refCast() const {
    return IRefPtr<S>(dynamic_cast<S *>(p_));
  }

 protected:
  void addRef() const {
    if (p_)
      p_->addRef();
  }

  void removeRef() {
    assert(getRef() > 0);

    if (p_) {
      p_->removeRef();

      if (getRef() == 0)
        free();
    }
  }

  void free() {
    delete p_;
  }

 protected:
  T *p_;
};

#endif
