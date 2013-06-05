#ifndef Array1D_H
#define Array1D_H

#include <PObject.h>
#include <CGeneric.h>

// Array of fundamental type objects

template<typename T>
class FArray1D : public PObject {
 public:
  FArray1D(int n=1) :
   data_(NULL), length(n) {
    alloc();
  }

  FArray1D(const T *data, int n) :
   data_(NULL), length(n) {
    alloc();

    memcpy(data_, data, length*sizeof(T));
  }

  FArray1D(const FArray1D &array) :
   PObject(), data_(NULL), length(array.length) {
    alloc();

    memcpy(data_, array.data_, length*sizeof(T));
  }

  FArray1D &operator=(const FArray1D &array) {
    if (length != array.length) {
      free();

      length = array.length;

      alloc();
    }

    memcpy(data_, array.data_, length*sizeof(T));

    return *this;
  }

 ~FArray1D() {
    // must be a fundamental type
    //CGENERIC_ASSERT(CGenericIsFundamental(T))
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

  void setValues(const std::vector<T> &values) {
    int l = (length < values.size() ? length : values.size());

    for (int i = 0; i < l; ++i)
      data_[i] = values[i];
  }

  //------

 public:
  struct Adder {
    Adder(FArray1D *p) :
     p_(p) {
    }

   ~Adder() {
      p_->setValues(values_);
    }

    Adder &operator,(const T &t) {
      return add(t);
    }

    Adder &add(const T &t) {
      values_.push_back(t);

      return *this;
    }

   private:
    FArray1D*      p_;
    std::vector<T> values_;
  };

  Adder operator<<(const T &t) {
    Adder adder(this);

    return adder.add(t);
  }

  //------

 private:
  void alloc() {
    data_ = new T [length];

    for (int i = 0; i < length; ++i)
      data_[i] = T();
  }

  void free() {
    delete [] data_;

    data_ = NULL;
  }

 private: T*  data_;
 public : int length;
};

//----------------

template<typename T>
class PArray1D : public PObject {
 public:
  typedef IRefPtr<T> PT;

  //------

  struct Ref {
    PArray1D *array;
    int       ind;

    Ref(PArray1D *array1, int ind1) :
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

  //------

 public:
  PArray1D(int n=1) :
   data_(NULL), length(n) {
    alloc();
  }

  PArray1D(const PArray1D &array) :
   PObject(), data_(NULL), length(array.length) {
    alloc();

    for (int i = 0; i < length; ++i)
      data_[i] = array.data_[i];
  }

  PArray1D &operator=(const PArray1D &array) {
    if (length != array.length) {
      free();

      length = array.length;

      alloc();
    }

    for (int i = 0; i < length; ++i)
      data_[i] = array.data_[i];

    return *this;
  }

 ~PArray1D() {
    // Must be a PObject derived class
    CGENERIC_ASSERT(CGenericIsDerived(T,PObject))
  }

  Ref operator[](int ind) {
    return Ref(this, ind);
  }

  PT get(int ind) const {
    assert(ind >= 0 && ind < length);

    return data_[ind];
  }

  void set(int ind, PT t) {
    assert(ind >= 0 && ind < length);

    data_[ind] = t;
  }

  void setValues(const std::vector<PT> &values) {
    int l = (length < values.size() ? length : values.size());

    for (int i = 0; i < l; ++i)
      data_[i] = values[i];
  }

  //------

 public:
  struct Adder {
    Adder(PArray1D *p) :
     p_(p) {
    }

   ~Adder() {
      p_->setValues(values_);
    }

    Adder &operator,(T t) {
      return add(new T(t));
    }

    Adder &add(PT t) {
      values_.push_back(t);

      return *this;
    }

   private:
    PArray1D*       p_;
    std::vector<PT> values_;
  };

  Adder operator<<(T t) {
    Adder adder(this);

    return adder.add(new T(t));
  }

  //------

 private:
  void alloc() {
    data_ = new PT [length];

    for (int i = 0; i < length; ++i)
      data_[i] = new T();
  }

  void free() {
    delete [] data_;

    data_ = NULL;
  }

 private: PT* data_;
 public : int length;
};

//----------------

template<typename T>
struct Array1DChooser {
  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    PArray1D<T>, FArray1D<T> >::Result Type;

  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    typename PArray1D<T>::Ref, T& >::Result Ref;
};

#define ARRAY1D(T,N) \
typedef typename Array1DChooser<T>::Type N; \
typedef IRefPtr< N, typename Array1DChooser<T>::Ref > N##P;

#endif
