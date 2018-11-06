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

// Returns true if opt is a valid long option
bool ValidLongOption(string& opt) {
  if(CountLeadingDashes(opt) == 2) {
    return true;
  } else {
    return false;
  }
}

// Returns true if opt is a valid short option
bool ValidShortOption(string& opt) {
  if(CountLeadingDashes(opt) == 1) {
    return true;
  } else {
    return false;
  }
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

// Get arguments following a specified option
vector<string> GetOptArgs(vector<string> argv, unsigned optIndex) {
  vector<string> optArgs;

  // Add option arguments to string
  for(unsigned i = optIndex + 1; i < argv.size(); i++) {
    if(argv[i][0] != '-') {
      optArgs.push_back(argv[i]);
    } else {
      break;
    }
  }

  // Return the vector
  return optArgs;
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
    cout << "ERROR, cannot convert '" << this->str << "' to integer.\n";
    exit(1);
  }
}

// Boolean conversion overload
bool Argument::GetBool(void) {
  stringstream ss(this->str);
  bool b;
  if(ss >> boolalpha >> b) {
    return b;
  } else {
    cout << "ERROR, cannot convert '" << this->str << "' to boolean.\n";
    exit(1);
  }
}

// Boolean conversion overload
double Argument::GetDouble(void) {
  stringstream ss(this->str);
  double d;
  if(ss >> d) {
    return d;
  } else {
    cout << "ERROR, cannot convert '" << this->str << "' to double.\n";
    exit(1);
  }
}


//========[OPTION SUBCLASS]======================================================================//

// Constructor without default arguments
Option::Option(string longIdent, char shortIdent, ArgType type, string desc) {
  this->Init(longIdent, shortIdent, type, desc);
}

// Constructor with string default arguments
Option::Option(string longIdent, char shortIdent, ArgType type, string desc, vector<string> def) {
  this->Init(longIdent, shortIdent, type, desc);
  this->SetArgs(def);
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

// Set arguments, overrides default arguments
void Option::SetArgs(vector<string> args) {
  this->args.clear();
  for(unsigned i = 0; i < args.size(); i++) {
    this->args.push_back(Argument(args[i]));
  }
}

// Indicate that the options has been specified
void Option::SetSpecified(void) {

  // Do not allow repeat specification of an option
  if(this->specified) {
    cout << "ERROR, option " << this->IDStr() << "already specified.\n";
    exit(1);
  } else {
    this->specified = true;
  }
}

// Short option match check
bool Option::OptMatches(char opt) {
  if(opt == this->shortIdent) {
    return true;
  } else {
    return false;
  }
}

// Long option match check
bool Option::OptMatches(string opt) {
  if(StripLeadingDashes(opt) == this->longIdent) {
    return true;
  } else {
    return false;
  }
}

// Parse from list of all command line arguments
void Option::Parse(vector<string> argv) {

  // Iterate until an option is found
  for(unsigned i = 0; i < argv.size(); i++) {

    // Long option
    if(CountLeadingDashes(argv[i]) == 2) {
      if(OptMatches(argv[i])) {
        this->SetSpecified();
        this->SetArgs(GetOptArgs(argv, i));
      }

    // Group of one or more short options
    } else if(CountLeadingDashes(argv[i]) == 1) {
      for(unsigned j = 1; j < argv[i].size(); j++) {
        if(this->OptMatches(argv[i][j])) {
          this->SetSpecified();
          if(j == argv[i].size() - 1) {
            this->SetArgs(GetOptArgs(argv, i));
          }
        }
      }
    }
  }
}

// Get arguments for a command line option
vector<Argument> Option::GetArguments(void) {

  if(this->args.size() == 0) {
    printf("Error, option '%s' has no arguments.\n", this->longIdent.c_str());
    exit(1);
  }

  return this->args;
}

// Assert that a conversion is valid
void Option::AssertArgRequestValid(ArgType requestedType) {

  // Check that request type matches option type
  assert(this->type == requestedType);

  // If not specified (default arguments) then assert (developer error)
  if(!this->specified) {
    assert(this->args.size() != 0);

  // If it was specified, then print a message (user error)
  } else {
    if(this->args.size() == 0) {
      cout << "ERROR option " << this->IDStr();
      cout << " expects at least one argument\n";
      exit(1);
    }
  }
}

// String array conversion
Option::operator vector<string>() {

  // Check that the argument request is valid
  AssertArgRequestValid(ARG_TYPE_STRING);

  // Convert all arguments
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

// Int array conversion
Option::operator vector<int>() {

  // Check that the argument request is valid
  AssertArgRequestValid(ARG_TYPE_INT);

  // Convert all arguments
  vector<int> convertedArgs;
  for(unsigned i = 0; i < this->args.size(); i++) {
    convertedArgs.push_back(this->args[i].GetInt());
  }

  return convertedArgs;
}

// Single string
Option::operator int() {
  AssertArgRequestValid(ARG_TYPE_INT);
  return this->args[0].GetInt();
}

// Returns a string representative of the option
string Option::IDStr(void) {
  stringstream ss;
  ss << "[--" << this->longIdent << ", -" << this->shortIdent << "]\n";
  return ss.str();
}


//========[OPTION PARSER SUBCLASS]===============================================================//

// Constructor clear options (and add a help option)
OptionParser::OptionParser(int argc, char **argv) {

  // Initialise local arguments
  for(int i = 1; i < argc; i++) {
    this->argv.push_back(string(argv[i]));
  }

  // Clear any present options
  this->options.clear();

  // Add the help option
  this->Add(Option(
    "help", 'h', ARG_TYPE_STRING,
    "Displays help message."));
}

// Add an option
void OptionParser::Add(Option opt) {
  opt.Parse(this->argv);
  this->options.push_back(opt);
}

// Get option with a given name
Option OptionParser::Get(string longIdent) {

  // Loop variables
  bool optLocated = false;
  unsigned optIndex = 0;

  // Iterate over all options looking for one that matches
  for(unsigned i = 0; i < this->options.size(); i++) {
    if(this->options[i].OptMatches(longIdent)) {
      if(!optLocated) {
        optIndex = i;
        optLocated = true;
      } else {
        printf("Error, option '%s' matches multiple registered options.\n", longIdent.c_str());
        exit(1);
      }
    }
  }

  // If we found an option, fantastic, otherwise, print an error message
  if(!optLocated) {
    printf("Error, option '%s' not registered with option parser.\n", longIdent.c_str());
    exit(1);
  } else {
    return this->options[optIndex];
  }
}

// Returns true if the option has been specified
bool OptionParser::Specified(string longIdent) {
  return this->Get(longIdent).Specified();
}
