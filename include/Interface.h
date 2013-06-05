#ifndef Interface_H
#define Interface_H

#define INTERFACE(C) \
class C { \
 public: \
  virtual ~C() { }

#define POBJECT_INTERFACE(C) \
class C : public PObject { \
 public: \
  virtual ~C() { }

#define IMETHOD(R,N) virtual R N = 0;

#endif
