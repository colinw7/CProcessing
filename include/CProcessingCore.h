#ifndef CProcessingCore_H
#define CProcessingCore_H

#include <cctype>
#include <cstdlib>
#include <cstddef>
#include <cmath>
#include <cassert>

#include <string>
#include <vector>

typedef bool boolean;

#include <Interface.h>
#include <IRefPtr.h>

#include <Array1D.h>
#include <Array2D.h>
#include <Array3D.h>
#include <ArrayList.h>
#include <PVector.h>
#include <String.h>

#define synchronized

#define PCLASS(N) \
  class N; \
  typedef IRefPtr<N> N##P; \
  class N : public PObject

#define PDERIVED_CLASS(N,D) \
  class N; \
  typedef IRefPtr<N> N##P; \
  class N : public D

#define SYNCHRONIZED(v) { \
  CThreadAutoMutex v##_auto_mutex(v##_mutex);

#define INSTANCEOF(i,C) \
  (dynamic_cast<C *>(i.getPtr()) != NULL)

//--------

class Float {
 public:
  explicit Float(float f) :
   f_(f) {
  }

  friend Float operator%(const Float &f1, int f2) {
    return f1 % Float(f2);
  }

  friend Float operator%(const Float &f1, float f2) {
    return f1 % Float(f2);
  }

  friend Float operator%(const Float &f1, const Float &f2) {
    int factor = (int) (f1.f_/f2.f_);

    float result = f1.f_ - (f2.f_*factor);

    return Float(result);
  }

  operator float() {
    return f_;
  }

 private:
  float f_;
};

//--------

class Random {
 public:
  Random() {
   srand48(time(0));
  }

  double nextGaussian() const {
    return drand48();
  }
};

class Exception {
 public:
  Exception() { }

  void printStackTrace() { }
};

#endif
