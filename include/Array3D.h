#ifndef Array3D_H
#define Array3D_H

template<typename T>
class Array3D {
 private:
  T   *data_;
  int  n1_, n2_, n3_;

 public:
  struct CRef2 {
    const Array3D &array;
    int            ind1;
    int            ind2;

    CRef2(const Array3D &array1, int ind11, int ind21) :
     array(array1), ind1(ind11), ind2(ind21) {
    }

    const T &operator[](int ind) const {
      return array.get(ind1, ind2, ind);
    }
  };

  struct Ref2 {
    Array3D &array;
    int      ind1;
    int      ind2;

    Ref2(Array3D &array1, int ind11, int ind21) :
     array(array1), ind1(ind11), ind2(ind21) {
    }

    T &operator[](int ind) {
      return array.get(ind1, ind2, ind);
    }
  };

  struct CRef1 {
    const Array3D &array;
    int            ind;

    CRef1(const Array3D &array1, int ind1) :
     array(array1), ind(ind1) {
    }

    CRef2 operator[](int ind1) const {
      return CRef2(array, ind, ind1);
    }
  };

  struct Ref1 {
    Array3D &array;
    int      ind;

    Ref1(Array3D &array1, int ind1) :
     array(array1), ind(ind1) {
    }

    Ref2 operator[](int ind1) {
      return Ref2(array, ind, ind1);
    }
  };

  Array3D(int n1=1, int n2=1, int n3=1) :
   data_(NULL), n1_(n1), n2_(n2), n3_(n3) {
    alloc();
  }

  Array3D(const Array3D &array) :
   data_(NULL), n1_(array.n1_), n2_(array.n2_), n3_(array.n3_) {
    alloc();

    memcpy(data_, array.data_, n1_*n2_*n3_*sizeof(T));
  }

  Array3D &operator=(const Array3D &array) {
    free();

    n1_ = array.n1_;
    n2_ = array.n2_;
    n3_ = array.n3_;

    alloc();

    memcpy(data_, array.data_, n1_*n2_*n3_*sizeof(T));

    return *this;
  }

  CRef1 operator[](int ind) const {
    return CRef1(*this, ind);
  }

  Ref1 operator[](int ind) {
    return Ref1(*this, ind);
  }

  const T &get(int i, int j, int k) const {
    return data_[i*n3_*n2_ + j*n3_ + k];
  }

  T &get(int i, int j, int k) {
    return data_[i*n3_*n2_ + j*n3_ + k];
  }

 private:
  void alloc() {
    data_ = new T [n1_*n2_*n3_];
  }

  void free() {
    delete [] data_;

    data_ = NULL;
  }
};

#endif
