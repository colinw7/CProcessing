#ifndef Array2D_H
#define Array2D_H

#include <PObject.h>
#include <CGeneric.h>

// 2D Array of fundamental type objects

template<typename T>
class FArray2D : public PObject {
 public:
  struct CRef {
    const FArray2D &array;
    int             ind;

    CRef(const FArray2D &array1, int ind1) :
     array(array1), ind(ind1) {
    }

    const T &operator[](int ind1) {
      return array.get(ind, ind1);
    }
  };

  struct Ref {
    FArray2D &array;
    int       ind;

    Ref(FArray2D &array1, int ind1) :
     array(array1), ind(ind1) {
    }

    T &operator[](int ind1) {
      return array.get(ind, ind1);
    }

    Ref &operator=(const FArray1D<T> &a) {
      for (int i = 0; i < a.length; ++i)
        array[ind][i] = a[i];

      return *this;
    }

    operator FArray1D<T>() {
      int len2 = array.length2();

      FArray1D<T> a(len2);

      for (int i = 0; i < len2; ++i)
        a[i] = array[ind][i];

      return a;
    }
  };

  //------

  FArray2D(int length1=1, int n2=1) :
   data_(NULL), length(length1), n2_(n2) {
    alloc();
  }

  FArray2D(const T *data, int length1, int n2) :
   data_(NULL), length(length1), n2_(n2) {
    alloc();

    memcpy(data_, data, length*n2_*sizeof(T));
  }

  FArray2D(const FArray2D &array) :
   PObject(array), data_(NULL), length(array.length), n2_(array.n2_) {
    alloc();

    memcpy(data_, array.data_, length*n2_*sizeof(T));
  }

  FArray2D &operator=(const FArray2D &array) {
    if (length != array.length || n2_ != array.n2_) {
      free();

      length = array.length;
      n2_    = array.n2_;

      alloc();
    }

    memcpy(data_, array.data_, length*n2_*sizeof(T));

    return *this;
  }

  ~FArray2D() {
    // must be a fundamental type
    //CGENERIC_ASSERT(CGenericIsFundamental(T))
  }

  CRef operator[](int ind) const {
    return CRef(*this, ind);
  }

  Ref operator[](int ind) {
    return Ref(*this, ind);
  }

  const T &get(int i, int j) const {
    return data_[i*n2_ + j];
  }

  T &get(int i, int j) {
    return data_[i*n2_ + j];
  }

  void set(int i, int j, const T &t) {
    data_[i*n2_ + j] = t;
  }

  int length2() const { return n2_; }

  void setValues(const std::vector< std::vector<T> > &values2) {
    int length1 = values2.size();

    assert(length1 == length);

    for (int i = 0; i < length1; ++i) {
      const std::vector<T> &values = values2[i];

      int n2 = values.size();

      assert(n2 == n2_);

      for (int j = 0; j < n2; ++j) {
        set(i, j, values[j]);
      }
    }
  }

  //------

 public:
  struct Adder {
    Adder(FArray2D *p) :
     p_(p), is_copy_(false) {
    }

    Adder(const Adder &a) :
     p_(a.p_), is_copy_(true), values_(a.values_), values2_(a.values2_) {
    }

   ~Adder() {
      if (! is_copy_) return;

      values2_.push_back(values_);

      p_->setValues(values2_);
    }

    Adder &operator,(const T &t) {
      return add(t);
    }

    Adder &add(const T &t) {
      values_.push_back(t);

      return *this;
    }

    Adder &operator|(const T &t) {
      values2_.push_back(values_);

      values_.clear();

      return add(t);
    }

   private:
    Adder &operator=(const Adder &a);

   private:
    FArray2D*                     p_;
    bool                          is_copy_;
    std::vector<T>                values_;
    std::vector< std::vector<T> > values2_;
  };

  Adder operator<<(const T &t) {
    Adder adder(this);

    return adder.add(t);
  }

  //-----

 private:
  void alloc() {
    data_ = new T [length*n2_];
  }

  void free() {
    delete [] data_;

    data_ = NULL;
  }

 private: T*  data_;
 public : int length;
 private: int n2_;
};

//----------------

template<typename T>
class PArray2D : public PObject {
 public:
  typedef IRefPtr<T> PT;

  //------

  struct Ref1 {
    PArray2D *array;
    int       ind1;
    int       ind2;

    Ref1(PArray2D *array_, int ind1_, int ind2_) :
     array(array_), ind1(ind1_), ind2(ind2_) {
    }

    Ref1 &operator=(PT t) {
      array->set(ind1, ind2, t);

      return *this;
    }

    operator PT() const {
      return array->get(ind1, ind2);
    }

    // p->
    PT operator->() {
      return array->get(ind1, ind2);
    }
  };

  //------

  struct Ref {
    PArray2D *array;
    int       ind;

    Ref(PArray2D *array1, int ind1) :
     array(array1), ind(ind1) {
    }

    Ref1 operator[](int ind1) {
      return Ref1(array, ind, ind1);
    }

    Ref &operator=(const PArray1D<PT> &a) {
      for (int i = 0; i < a.length; ++i)
        array[ind][i] = a[i];

      return *this;
    }

    operator PArray1D<PT>() {
      int len2 = array->length2();

      PArray1D<PT> a(len2);

      for (int i = 0; i < len2; ++i)
        a[i] = (*array)[ind][i];

      return a;
    }
  };

  //------

 public:
  PArray2D(int length1=1, int n2=1) :
   data_(NULL), length(length1), n2_(n2) {
    alloc();
  }

  PArray2D(const PArray2D &array) :
   PObject(array), data_(NULL), length(array.length), n2_(array.n2_) {
    alloc();

    for (int i = 0; i < length*n2_; ++i)
      data_[i] = array.data_[i];
  }

  PArray2D &operator=(const PArray2D &array) {
    if (length != array.length || n2_ != array.n2_) {
      free();

      length = array.length;
      n2_    = array.n2_;

      alloc();
    }

    for (int i = 0; i < length*n2_; ++i)
      data_[i] = array.data_[i];

    return *this;
  }

 ~PArray2D() {
    // Must be a PObject derived class
    //CGENERIC_ASSERT(CGenericIsDerived(T,PObject))
  }

  Ref operator[](int ind) {
    return Ref(this, ind);
  }

  PT get(int i, int j) {
    assert(i >= 0 && i < length && j >= 0 && j < n2_);

    return data_[i*n2_ + j];
  }

  void set(int i, int j, PT t) {
    assert(i >= 0 && i < length && j >= 0 && j < n2_);

    data_[i*n2_ + j] = t;
  }

  int length2() const { return n2_; }

  void setValues(const std::vector< std::vector<PT> > &values2) {
    int length1 = values2.size();

    assert(length1 == length);

    for (int i = 0; i < length1; ++i) {
      const std::vector<PT> &values = values2[i];

      int n2 = values.size();

      assert(n2 == n2_);

      for (int j = 0; j < n2; ++j) {
        set(i, j, values[j]);
      }
    }
  }

  //------

 public:
  struct Adder {
    Adder(PArray2D *p) :
     p_(p), is_copy_(false) {
    }

    Adder(const Adder &a) :
     p_(a.p_), is_copy_(true), values_(a.values_), values2_(a.values2_) {
    }

   ~Adder() {
      if (! is_copy_) return;

      values2_.push_back(values_);

      p_->setValues(values2_);
    }

    Adder &operator,(T t) {
      return add(new T(t));
    }

    Adder &add(PT t) {
      values_.push_back(t);

      return *this;
    }

    Adder &operator|(T t) {
      values2_.push_back(values_);

      values_.clear();

      return add(new T(t));
    }

   private:
    Adder &operator=(const Adder &a);

   private:
    PArray2D*                      p_;
    bool                           is_copy_;
    std::vector<PT>                values_;
    std::vector< std::vector<PT> > values2_;
  };

  Adder operator<<(T t) {
    Adder adder(this);

    return adder.add(new T(t));
  }

  //-----

 private:
  void alloc() {
    data_ = new PT [length*n2_];

    for (int i = 0; i < length*n2_; ++i)
      data_[i] = new T();
  }

  void free() {
    delete [] data_;

    data_ = NULL;
  }

 private: PT* data_;
 public : int length;
 private: int n2_;
};

//----------------

template<typename T>
struct Array2DChooser {
  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    PArray2D<T>, FArray2D<T> >::Result Type;

  typedef typename CGenericIfElse< CGenericIsDerived(T,PObject),
    typename PArray2D<T>::Ref, typename FArray2D<T>::Ref >::Result Ref;
};

#define ARRAY2D(T,N) \
typedef typename Array2DChooser<T>::Type N; \
typedef IRefPtr< N, typename Array2DChooser<T>::Ref > N##P;

#endif
