#include <optparse.hpp>
using namespace std;


// String constructor
Argument::Argument(string str) {
  this->str = str;
}


// Integer constructor
Argument::Argument(int i) {
  stringstream ss;
  ss << i;
  this->str = ss.str();
}


// Boolean constructor
Argument::Argument(bool b) {
  stringstream ss;
  ss << boolalpha << b;
  this->str = ss.str();
}


// Floating point constructor
Argument::Argument(double d) {
  stringstream ss;
  ss << d;
  this->str = ss.str();
}


// String conversion overload
string Argument::GetString(void) {
  return this->str;
}


// Integer conversion overload
int Argument::GetInt(void) {
  stringstream ss(this->str);
  int i;
  if(ss >> i) {
    return i;
  } else {
    throw ArgumentConversionException();
  }
}


// Boolean conversion overload
bool Argument::GetBool(void) {
  stringstream ss(this->str);
  bool b;
  if(ss >> boolalpha >> b) {
    return b;
  } else {
    throw ArgumentConversionException();
  }
}


// Double conversion overload
double Argument::GetDouble(void) {
  stringstream ss(this->str);
  double d;
  if(ss >> d) {
    return d;
  } else {
    throw ArgumentConversionException();
  }
}
