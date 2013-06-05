#ifndef RefPtr_H
#define RefPtr_H

template<typename T, typename AT=int>
class RefPtr {
 protected:
  template<typename S, typename AS>
  friend class RefPtr;

 public:
  RefPtr(T *p=NULL) :
   p_(p), count_(new int(1)) {
  }

  RefPtr(const RefPtr &p) :
   p_(p.p_), count_(p.count_) {
    addRef();
  }

  RefPtr &operator=(const RefPtr &p) {
    if (p.p_ == p_) return *this;

    p.addRef();

    removeRef();

    p_     = p.p_;
    count_ = p.count_;

    return *this;
  }

 ~RefPtr() {
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
  operator RefPtr<S,AT>() const {
    return RefPtr<S,AT>((S *) p_, count_);
  }

  // subscript
  const AT &operator[](int i) const {
    return (*getPtr())[i];
  }

  AT &operator[](int i) {
    return (*getPtr())[i];
  }

  T *getPtr() const {
    assert(p_ != 0 && getRef() > 0);

    return p_;
  }

  template<typename S>
  RefPtr &operator=(const RefPtr<S> &p) {
    if (p.p_ == p_)
      return *this;

    p.addRef();

    removeRef();

    p_     = p.p_;
    count_ = p.count_;

    return *this;
  }

  RefPtr &operator=(const T *p) {
    if (p == p_)
      return *this;

    removeRef();

    p_     = const_cast<T *>(p);
    count_ = new int(0);

    addRef();

    return *this;
  }

  RefPtr &operator=(T *p) {
    if (p == p_)
      return *this;

    removeRef();

    p_     = p;
    count_ = new int(0);

    addRef();

    return *this;
  }

  bool operator==(const RefPtr &p) const {
    return p_ == p.p_;
  }

  bool operator!=(const RefPtr &p) const {
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

  void uniquify() {
    if (getRef() > 1) {
      p_ = p_->dup();

      count_ = new int(0);

      addRef();
    }
  }

  int getRef() const {
    return *count_;
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
  RefPtr<S>
  refCast() const {
    return RefPtr<S>(dynamic_cast<S *>(p_), count_);
  }

 protected:
  RefPtr(T *p, int *count) :
   p_(p), count_(count) {
    addRef();
  }

  void addRef() const {
    RefPtr *th = const_cast<RefPtr *>(this);

    ++(*th->count_);
  }

  void removeRef() {
    assert(*count_ > 0);

    --(*count_);

    if (*count_ == 0)
      free();
  }

  void free() {
    assert(count_);

    delete p_;
    delete count_;
  }

 protected:
  T   *p_;
  int *count_;
};

extern RefPtr<long> null;

#endif
