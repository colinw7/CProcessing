#include <CProcessingCore.h>
#include <CStrUtil.h>

PObjectP null;

String::
String(int i)
{
  s_ = toStdString(i);
}

String::
String(double r)
{
  s_ = CStrUtil::toString(r);
}

std::string
String::
toStdString(int i)
{
  return CStrUtil::toString(i);
}

std::string
String::
toStdString(float f)
{
  return CStrUtil::toString(f);
}

std::string
String::
toStdString(double f)
{
  return CStrUtil::toString(f);
}
