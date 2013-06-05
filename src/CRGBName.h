#ifndef CRGB_NAME_H
#define CRGB_NAME_H

#include <string>
#include <map>

class CRGBName {
  typedef std::map<std::string, int> ColorMap;

 private:
  static ColorMap color_map_;
  static bool     color_map_set_;

 public:
  static bool lookup(const std::string &name, double *r, double *g, double *b, double *a=NULL);
  static bool lookup(const std::string &name, float *r, float *g, float *b, float *a=NULL);
};

#endif
