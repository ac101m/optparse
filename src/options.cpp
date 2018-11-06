#include "options.hpp"


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


//========[OPTION SUBCLASS]======================================================================//

// Constructor with default arguments
Option::Option(string longIdent, char shortIdent, string usage, vector<string> def) {

  // Do the initialisation
  this->shortIdent = shortIdent;
  this->longIdent = longIdent;
  this->args = def;
  this->usage = usage;

  // Not specified yet.
  this->specified = false;
}

// Set arguments, overrides default arguments
void Option::SetArguments(vector<string> args) {
  this->args = args;
}

// Indicate that the options has been specified
void Option::SetSpecified(void) {

  // Do not allow repeat specification of an option
  if(this->specified) {
    printf("Error, option '%s' already specified.\n", this->longIdent.c_str());
    exit(1);
  } else {
    this->specified = true;
  }
}

// Parse from list of all command line arguments
void Option::Parse(vector<string> argv) {

  // Iterate until an option is found
  for(unsigned i = 0; i < argv.size(); i++) {

    // Long option
    if(CountLeadingDashes(argv[i]) == 2) {
      if(OptMatches(StripLeadingDashes(argv[i]))) {
        this->SetSpecified();
        this->SetArguments(GetOptArgs(argv, i));
      }

      // Group of one or more short options
    } else if(CountLeadingDashes(argv[i]) == 1) {
      for(unsigned j = 1; i < argv[i].size(); j++) {
        if(this->OptMatches(argv[i][j])) {
        this->SetSpecified();
          if(j == argv[i].size() - 1) {
              this->SetArguments(GetOptArgs(argv, i));
          }
        }
      }
    }
  }
}

// Get arguments for a command line option
vector<string> Option::GetArguments(void) {

  if(this->args.size() == 0) {
    printf("Error, option '%s' has no arguments.\n", this->longIdent.c_str());
    exit(1);
  }

  return this->args;
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
  if(opt == this->longIdent) {
    return true;
  } else {
    return false;
  }
}


//========[OPTION PARSER SUBCLASS]===============================================================//

// Constructor clear options (and add a help option)
OptionParser::OptionParser(int argc, char **argv) {

  // Initialise
  for(int i = 1; i < argc; i++) {
    this->argv.push_back(string(argv[i]));
  }

  // Clear any present options
  this->options.clear();

  // Add the help option
  this->AddOption("help", 'h',
                  "Displays help message.");
}

// Instantiate and add an option
void OptionParser::AddOption(string longIdent, char shortIdent, string usage, vector<string> def) {
  Option opt = Option(longIdent, shortIdent, usage, def);
  opt.Parse(this->argv);
  this->options.push_back(opt);
}

// Instantiate and add an option
void OptionParser::AddOption(string longIdent, char shortIdent, string usage, string def) {
  vector<string> defaultArgs;
  defaultArgs.push_back(def);
  Option opt = Option(longIdent, shortIdent, usage, defaultArgs);
  opt.Parse(this->argv);
  this->options.push_back(opt);
}

// Instantiate and add an option with no arguments
void OptionParser::AddOption(string longIdent, char shortIdent, string usage) {
  vector<string> defaultArgs;
  Option opt = Option(longIdent, shortIdent, usage, defaultArgs);
  opt.Parse(this->argv);
  this->options.push_back(opt);
}

// Add an option
void OptionParser::AddOption(Option opt) {
  this->options.push_back(opt);
}

// Get option with a given name
Option OptionParser::GetOption(string longIdent) {

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

// Get option with a given name
Option OptionParser::GetOption(char shortIdent) {

  // Loop variables
  bool optLocated = false;
  unsigned optIndex = 0;

  // Iterate over all options looking for one that matches
  for(unsigned i = 0; i < this->options.size(); i++) {
    if(this->options[i].OptMatches(shortIdent)) {
      if(!optLocated) {
        optIndex = i;
        optLocated = true;
      } else {
        printf("Error, option '%c' matches multiple registered options.\n", shortIdent);
        exit(1);
      }
    }
  }

  // If we found an option, fantastic, otherwise, print an error message
  if(!optLocated) {
    printf("Error, option '%c' not registered with option parser.\n", shortIdent);
    exit(1);
  } else {
    return this->options[optIndex];
  }
}

// Returns true if the option has been specified
bool OptionParser::Specified(string longIdent) {
  return this->GetOption(longIdent).Specified();
  return false;
}
