#include <optparse.hpp>
#include <assert.h>
#include <iostream>
using namespace std;


// Constructor without default arguments
Option::Option(string longIdent, char shortIdent, ArgType type, string desc) {
  this->Init(longIdent, shortIdent, type, desc);
}


// Constructor for strings
Option::Option(string longIdent, char shortIdent, ArgType type, string desc, vector<string> def) {
  this->Init(longIdent, shortIdent, type, desc);

  // Set default arguments
  for(unsigned i = 0; i < def.size(); i++) {
    this->args.push_back(Argument(def[i]));
  }
}


// Init routine, used for common constructor code
void Option::Init(string longIdent, char shortIdent, ArgType type, string desc) {

  // Do the initialisation
  this->shortIdent = shortIdent;
  this->longIdent = longIdent;
  this->type = type;
  this->desc = desc;

  // Not specified yet.
  this->specified = false;
}


// Short option match check, ignores leading dashes
bool Option::OptMatches(char opt) {
  if(opt == this->shortIdent) {
    return true;
  } else {
    return false;
  }
}


// Long option match check, ignores leading dashes
bool Option::OptMatches(string opt) {
  if(StripLeadingDashes(opt) == this->longIdent) {
    return true;
  } else {
    return false;
  }
}


// Append an argument, overriding default arguments
void Option::Specify(vector<string> args) {
  if(this->specified) {
    cout << "ERROR, option " << this->IDStr() << " multiply specified.\n";
    exit(1);
  } else {
    this->args.clear();
    this->specified = true;
    for(unsigned i = 0; i < args.size(); i++) {
      this->args.push_back(Argument(args[i]));
    }
  }
}


// Assert that a conversion is valid
void Option::AssertArgRequestValid(ArgType requestedType) {

  // Check that request type matches option type
  assert(this->type == requestedType);

  // If not specified (default arguments) then assert (developer error)
  if(!this->specified) {
    assert(this->args.size() != 0);
  } else {
    if(this->args.size() == 0) {
      cout << "ERROR, option " << this->IDStr();
      cout << " expects at least one argument.\n";
      exit(1);
    }
  }
}


// Returns a string representative of the option
string Option::IDStr(void) {
  stringstream ss;
  ss << "[-" << this->shortIdent << ", --" << this->longIdent << "]";
  return ss.str();
}


// Generate argument type string
string Option::ArgStr(void) {
  switch(this->type) {
    case ARG_TYPE_INT: return "<integer args...>";
    case ARG_TYPE_BOOL: return "<boolean args...>";
    case ARG_TYPE_FLOAT: return "<decimal args...>";
    case ARG_TYPE_STRING: return "<string args...>";
    case ARG_TYPE_VOID: return "";
    default:
      cout << "ERROR, option " << this->IDStr() << " invalid typeid.\n";
      exit(1);
  }
}


// Generate syntax string
string Option::SyntaxStr(void) {
  stringstream ss;
  ss << "Syntax: " << this->IDStr();
  ss << " " << this->ArgStr();
  return ss.str();
}


// Generate help string
string Option::HelpStr(void) {
  stringstream ss;
  ss << this->longIdent << ":\n";
  ss << "  " << this->desc << "\n";
  ss << "  " << this->SyntaxStr();
  return ss.str();
}


//====[CONVERSION METHODS]===================================================//

// String list
Option::operator vector<string>() {
  AssertArgRequestValid(ARG_TYPE_STRING);
  vector<string> convertedArgs;
  for(unsigned i = 0; i < this->args.size(); i++) {
    convertedArgs.push_back(this->args[i].GetString());
  }
  return convertedArgs;
}


// Single string
Option::operator string() {
  AssertArgRequestValid(ARG_TYPE_STRING);
  return this->args[0].GetString();
}


// Integer list
Option::operator vector<int>() {
  AssertArgRequestValid(ARG_TYPE_INT);
  vector<int> convertedArgs;
  for(unsigned i = 0; i < this->args.size(); i++) {
    try {
      convertedArgs.push_back(this->args[i].GetInt());
    } catch(ArgumentConversionException& e) {
      cout << "ERROR, option " << this->IDStr();
      cout << " expects integer arguments.\n";
      exit(1);
    }
  }
  return convertedArgs;
}


// Single integer
Option::operator int() {
  AssertArgRequestValid(ARG_TYPE_INT);
  try {
    return this->args[0].GetInt();
  } catch(ArgumentConversionException& e) {
    cout << "ERROR, option " << this->IDStr();
    cout << " Expects integer arguments.\n";
    exit(1);
  }
}


// Boolean list
Option::operator vector<bool>() {
  AssertArgRequestValid(ARG_TYPE_BOOL);
  vector<bool> convertedArgs;
  for(unsigned i = 0; i < this->args.size(); i++) {
    try {
      convertedArgs.push_back(this->args[i].GetBool());
    } catch(ArgumentConversionException& e) {
      cout << "ERROR, option " << this->IDStr();
      cout << " Expects boolean arguments ('true' or 'false').\n";
      exit(1);
    }
  }
  return convertedArgs;
}


// Single boolean
Option::operator bool() {
  AssertArgRequestValid(ARG_TYPE_BOOL);
  try {
    return this->args[0].GetBool();
  } catch(ArgumentConversionException& e) {
    cout << "ERROR, option " << this->IDStr();
    cout << " Expects boolean arguments ('true' or 'false').\n";
    exit(1);
  }
}


// List of floating point numbers
Option::operator vector<double>() {
  AssertArgRequestValid(ARG_TYPE_FLOAT);
  vector<double> convertedArgs;
  for(unsigned i = 0; i < this->args.size(); i++) {
    try {
      convertedArgs.push_back(this->args[i].GetDouble());
    } catch(ArgumentConversionException& e) {
      cout << "ERROR, option " << this->IDStr();
      cout << " Expects floating point number.\n";
      exit(1);
    }
  }
  return convertedArgs;
}


// Single floating point numbers
Option::operator double() {
  AssertArgRequestValid(ARG_TYPE_FLOAT);
  try {
    return this->args[0].GetDouble();
  } catch(ArgumentConversionException& e) {
    cout << "ERROR, option " << this->IDStr();
    cout << " Expects floating point number.\n";
    exit(1);
  }
}

// Convert to vector of doubles to vector of floats
Option::operator vector<float>() {
  vector<double> dvec = *this;
  vector<float> fvec;
  fvec.reserve(dvec.size());
  for(unsigned i = 0; i < dvec.size(); i++) {
    fvec.push_back(dvec[i]);
  }
  return fvec;
}


// Convert from double to float
Option::operator float() {
  double d = *this;
  return d;
}
