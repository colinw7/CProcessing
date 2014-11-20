#ifndef ArrayList_H
#define ArrayList_H

template<typename T>
class FArrayList : public PObject {
 public:
  typedef IRefPtr<FArrayList, T&> FArrayListP;

  class Iterator {
   public:
    Iterator(FArrayList &l) :
     l_(l), pos_(0) {
    }

    bool hasNext() const {
      return pos_ < l_.size();
    }

    T &next() {
      return l_.get(pos_++);
    }

    void remove() {
      l_.remove(pos_);
    }

   private:
    FArrayList &l_;
    int         pos_;
  };

 private:
  std::vector<T> data_;

 public:
  FArrayList() { }

 ~FArrayList() {
    // must be a fundamental type
    //CGENERIC_ASSERT(CGenericIsFundamental(T))
  }

  void add(const T &t) {
    data_.push_back(t);
  }

  void addAll(FArrayListP a) {
    uint num = a->size();

    for (uint i = 0; i < num; ++i)
      add(a->get(i));
  }

  int size() const {
    return (int) data_.size();
  }

  const T &operator[](int ind) const {
    return data_[ind];
  }

  T &operator[](int ind) {
    return data_[ind];
  }

  const T &get(int ind) const {
    return data_[ind];
  }

  T &get(int ind) {
    return data_[ind];
  }

  void set(int ind, const T &t) {
    data_[ind] = t;
  }

  void clear() {
    data_.clear();
  }

  T &remove(int i) {
    T &t = data_[i];

    for (int i1 = i + 1; i1 < size(); ++i1)
      data_[i1 - 1] =  data_[i1];

    return t;
  }

  T &remove(T &t) {
    for (int i = 0; i < size(); ++i)
      if (data_[i] == t)
        return remove(i);

    return data_[0];
  }

  bool isEmpty() const {
    return data_.empty();
  }

  T &lastElement() {
    return data_.back();
  }

  Iterator iterator() {
    return Iterator(*this);
  }
};

template<typename T>
class PArrayList : public PObject {
 public:
  typedef IRefPtr<T> PT;

  //------

  struct Ref {
    PArrayList *array;
    int         ind;

    Ref(PArrayList *array1, int ind1) :
     array(array1), ind(ind1) {
    }

    Ref &operator=(PT t) {
      array->set(ind, t);

      return *this;
    }

    operator PT() const {
      return array->get(ind);
    }

    // p->
    PT operator->() {
      return array->get(ind);
    }
  };

  typedef IRefPtr<PArrayList, typename PArrayList::Ref> PArrayListP;

  //------

  class Iterator {
   public:
    Iterator(PArrayList *l) :
     l_(l), pos_(0) {
    }

    bool hasNext() const {
      return pos_ < l_->size();
    }

    PT next() {
      return l_->get(pos_++);
    }

    void remove() {
      l_->remove(pos_ - 1);

      --pos_;
    }

   private:
    PArrayList *l_;
    int         pos_;
  };

  //------

 public:
  PArrayList() { }

 ~PArrayList() {
    // Must be a PObject derived class
    //CGENERIC_ASSERT(CGenericIsDerived(T,PObject))
  }

  void add(PT t) {
    data_.push_back(t);
  }

  void addAll(PArrayListP a) {
    uint num = a->size();

    for (uint i = 0; i < num; ++i)
      add(a->get(i));
  }

  int size() const {
    return (int) data_.size();
  }

  Ref operator[](int ind) {
    return Ref(this, ind);
  }

  PT get(int ind) {
    return data_[ind];
  }

  void set(int ind, PT t) {
    data_[ind] = t;
  }

  void clear() {
    data_.clear();
  }

  PT remove(int i) {
    PT t = data_[i];

    for (int i1 = i + 1; i1 < size(); ++i1)
      data_[i1 - 1] =  data_[i1];

    data_.pop_back();

    return t;
  }

  PT remove(PT t) {
    for (int i = 0; i < size(); ++i)
      if (data_[i] == t)
        return remove(i);

    return data_[0];
  }

  bool isEmpty() const {
    return data_.empty();
  }

  PT lastElement() {
    return data_.back();
  }

  Iterator iterator() {
    return Iterator(this);
  }

 private:
  std::vector<PT> data_;
};

//----------------

template<typename T>
struct ArrayListChooser {
  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    PArrayList<T>, FArrayList<T> >::Result Type;

  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    typename PArrayList<T>::Ref, T& >::Result Ref;
};

#define ARRAY_LIST(T,N) \
typedef typename ArrayListChooser<T>::Type N; \
typedef IRefPtr< N, typename ArrayListChooser<T>::Ref > N##P;

#endif
