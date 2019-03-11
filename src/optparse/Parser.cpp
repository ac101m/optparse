#include <optparse.hpp>
#include <assert.h>
#include <iostream>
using namespace std;


// Constructor clear options (and add a help option)
void OptionParser::Init(int argc, char **argv) {

  // Get name from
  this->name = string(argv[0]);

  // Initialise local arguments and program name
  for(int i = 1; i < argc; i++) {
    this->argv.push_back(string(argv[i]));
  }

  // Indicate that the option is not finalised
  this->finalised = false;

  // Clear any present options
  this->options.clear();

  // Add the help option
  this->Add(Option(
    "help", 'h', ARG_TYPE_STRING,
    "Displays help messages for all commands."
  ));
}


// Constructor without program description
OptionParser::OptionParser(int argc, char **argv) {
  this->Init(argc, argv);
}


// Constructor without program description
OptionParser::OptionParser(int argc, char **argv, string desc) {
  this->Init(argc, argv);
  this->desc = desc;
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


// Generate help output
void OptionParser::DoHelpOutput(void) {

  // Print out name of program
  cout << "PROGRAM DESCRIPTION:\n";
  cout << this->desc << "\n\n";

  // Get the specific command for which help was requested
  Option& helpOption = this->FindOption("help");

  // Print help output for each option
  cout << "OPTIONS:\n";
  for(unsigned i = 0; i < this->options.size(); i++) {
    cout << options[i].HelpStr() << "\n\n";
  }

  // Help command terminates program
  exit(0);
}


// Parse all command line options, messy could do with a refactor
void OptionParser::Parse(void) {

  // Assign option arguments to options
  for(unsigned i = 0; i < this->argv.size(); i++) {
    if(CountLeadingDashes(this->argv[i]) == 2) {
      this->ParseLongOption(i);
    } else if(CountLeadingDashes(this->argv[i]) == 1) {
      this->ParseShortOptionBlock(i);
    } else {

      // The first argument must be an option
      if(i == 0) {
        cout << "ERROR, expected option before '" << this->argv[i] << "'.\n";
        exit(1);
      }
    }
  }

  // Check if the help option is defined
  if(this->FindOption("help").Specified()) {
    this->DoHelpOutput();
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
