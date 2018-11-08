#include "options.hpp"
#include <assert.h>
#include <iostream>


#include <stdio.h>



//========[GENERAL LOCAL UTILITY FUNCTIONS]======================================================//

// Counts number of leading dashes in a string. Duh.
unsigned CountLeadingDashes(string& opt) {

  // Count leading dashes
  unsigned leadingDashes = 0;
  for(unsigned i = 0; i < opt.size(); i++) {
    if(opt[i] == '-') {
      leadingDashes++;
    } else {
      break;
    }
  }

  // Return the number of dashes
  return leadingDashes;
}


// Strip leading dashes from a string
string StripLeadingDashes(string& opt) {
  string r;
  for(unsigned i = 0; i < opt.size(); i++) {
    if(i >= CountLeadingDashes(opt)) {
      r += opt[i];
    }
  }
  return r;
}



//========[OPTION ARGUMENT SUBCLASS]=============================================================//

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


// Boolean conversion overload
double Argument::GetDouble(void) {
  stringstream ss(this->str);
  double d;
  if(ss >> d) {
    return d;
  } else {
    throw ArgumentConversionException();
  }
}



//========[OPTION SUBCLASS]======================================================================//


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


// Returns a string representative of the option
string Option::IDStr(void) {
  stringstream ss;
  ss << "[--" << this->longIdent << ", -" << this->shortIdent << "]";
  return ss.str();
}



//========[OPTION PARSER SUBCLASS]===============================================================//


// Constructor clear options (and add a help option)
OptionParser::OptionParser(int argc, char **argv) {

  // Initialise local arguments
  for(int i = 1; i < argc; i++) {
    this->argv.push_back(string(argv[i]));
  }

  // Indicate that the option is not finalised
  this->finalised = false;

  // Clear any present options
  this->options.clear();

  // Add the help option (presently does nothing)
  this->Add(Option(
    "help", 'h', ARG_TYPE_STRING,
    "Displays help message.",
    {"all"}
  ));
}


// Add an option
void OptionParser::Add(Option opt) {
  assert(this->finalised != true);
  this->options.push_back(opt);
}


// Find an option with a given long ident
Option& OptionParser::FindOption(string longIdent) {

  // Collect all options that match the given long identifier
  vector<unsigned> matchIndices;
  for(unsigned i = 0; i < this->options.size(); i++) {
    if(this->options[i].OptMatches(longIdent)) {
      matchIndices.push_back(i);
    }
  }

  // Multiple definition of an option is programmer error
  assert(matchIndices.size() <= 1 /* short ident */);

  // Is the option found or not
  if(matchIndices.size() == 0) {
    cout << "ERROR, option '" << longIdent << "' not recognised.\n";
    exit(1);
  } else {
    return this->options[matchIndices[0]];
  }
}


// Find an option with a given short ident
Option& OptionParser::FindOption(char shortIdent) {

  // Collect all options that match the given long identifier
  vector<unsigned> matchIndices;
  for(unsigned i = 0; i < this->options.size(); i++) {
    if(this->options[i].OptMatches(shortIdent)) {
      matchIndices.push_back(i);
    }
  }

  // Multiple definition of an option is programmer error
  assert(matchIndices.size() <= 1 /* short ident */);

  // Is the option found or not
  if(matchIndices.size() == 0) {
    cout << "ERROR, option '" << shortIdent << "' not recognised.\n";
    exit(1);
  } else {
    return this->options[matchIndices[0]];
  }
}


// Parse a long option
void OptionParser::ParseLongOption(unsigned optIndex) {
  string optID = this->argv[optIndex];
  assert(CountLeadingDashes(optID) == 2);

  // Extract option arguments
  vector<string> optArgs;
  for(unsigned i = optIndex; i < this->argv.size(); i++) {
    if(i != optIndex) {
      if(CountLeadingDashes(this->argv[i])) break;
      else optArgs.push_back(this->argv[i]);
    }
  }

  // Get the matching option
  FindOption(optID).Specify(optArgs);
}


// Parse a block of short options option
void OptionParser::ParseShortOptionBlock(unsigned optIndex) {
  string optBlock = this->argv[optIndex];
  assert(CountLeadingDashes(optBlock) == 1);

  // Extract option arguments
  vector<string> optArgs;
  for(unsigned i = optIndex; i < this->argv.size(); i++) {
    if(i != optIndex) {
      if(CountLeadingDashes(this->argv[i])) break;
      else optArgs.push_back(this->argv[i]);
    }
  }

  // Specify all of the short options, only the last takes arguments
  optBlock = StripLeadingDashes(optBlock);
  for(unsigned i = 0; i < optBlock.size(); i++) {
    if(i == optBlock.size() - 1) {
      FindOption(optBlock[i]).Specify(optArgs);
    } else {
      FindOption(optBlock[i]).Specify({});
    }
  }
}


// Parse all command line options, messy could do with a refactor
void OptionParser::Parse(void) {

  // Check that first argument has leading dashes
  if(CountLeadingDashes(this->argv[0]) == 0) {
    cout << "ERROR, expected option before '" << this->argv[0] << "'.\n";
    exit(1);
  }

  // Assign option arguments to options
  for(unsigned i = 0; i < this->argv.size(); i++) {
    if(CountLeadingDashes(this->argv[i]) == 2) {
      this->ParseLongOption(i);
    } else if(CountLeadingDashes(this->argv[i]) == 1) {
      this->ParseShortOptionBlock(i);
    }
  }
}


// Get option with a given name
Option OptionParser::Get(string longIdent) {

  // The first call to get finalises the parser and parses arguments
  if(!this->finalised) {
    this->Parse();
    this->finalised = true;
  }

  // Loop variables
  bool optLocated = false;
  unsigned optIndex = 0;

  // Iterate over all options looking for one that matches
  return this->FindOption(longIdent);
}


// Returns true if the option has been specified
bool OptionParser::Specified(string longIdent) {
  return this->Get(longIdent).Specified();
}
