#ifndef String_H
#define String_H

#include <PObject.h>

class String;

typedef IRefPtr<String> StringP;

class String : public PObject {
 private:
  std::string s_;

 public:
  String() { }

  explicit String(const char *s) : s_(s) { }

  String(const char *s, int len) : s_(s, len) { }

  explicit String(const std::string &s) : s_(s) { }

  String(const String &s) : PObject(s), s_(s.s_) { }

  explicit String(int i);
  explicit String(double r);

  String &operator=(const String &s) {
    s_ = s.s_;

    return *this;
  }

  operator const char *() {
    return s_.c_str();
  }

  friend StringP operator+(StringP s1, StringP s2) {
    return new String(s1->s_ + s2->s_);
  }

  friend StringP operator+(int i1, StringP s2) {
    return operator+(new String(i1), s2);
  }

  friend StringP operator+(float f1, StringP s2) {
    return operator+(new String(f1), s2);
  }

  friend StringP operator+(double f1, StringP s2) {
    return operator+(new String(f1), s2);
  }

  friend StringP operator+(const char *s1, StringP s2) {
    return new String(s1 + s2->s_);
  }

  friend StringP operator+(StringP s1, int i2) {
    return operator+(new String(s1), new String(i2));
  }

  friend StringP operator+(StringP s1, float f2) {
    return operator+(new String(s1), new String(f2));
  }

  friend StringP operator+(StringP s1, double f2) {
    return operator+(new String(s1), new String(f2));
  }

  friend StringP operator+(StringP s1, const char *s2) {
    return new String(s1->s_ + s2);
  }

  int length() const { return s_.size(); }

  const std::string &str() const { return s_; }

  const char *c_str() const { return s_.c_str(); }

  friend std::ostream &operator<<(std::ostream &os, StringP s) {
    os << s->str();

    return os;
  }

  static StringP begin(const char *s="") { return new String(s); }

 private:
  static std::string toStdString(int i);
  static std::string toStdString(float f);
  static std::string toStdString(double f);
};

#endif
