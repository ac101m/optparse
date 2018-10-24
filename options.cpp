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


//========[OPTION SUBCLASS]======================================================================//


// Constructor with default arguments
Option::Option(ArgType_t type, string longIdent, char shortIdent, string usage, vector<string> def) {
    if(type == OPTION_TYPE_FLAG) {
        printf("Error, OPTION_TYPE_FLAG does not expect default arguments.\n");
        exit(1);
    }

    // Do the initialisation
    this->type = type;
    this->shortIdent = shortIdent;
    this->longIdent = longIdent;
    this->arguments = def;
    this->usage = usage;

    // Not specified yet.
    this->specified = false;
    this->required = false;         // Default arguments specified

    // Do a check to make sure the default arguments are valid
    this->AssertArgsValid();
}


// Constructor without default arguments
Option::Option(ArgType_t type, string longIdent, char shortIdent, string usage) {

    // Do the initialisation
    this->type = type;
    this->shortIdent = shortIdent;
    this->longIdent = longIdent;
    this->arguments.clear();
    this->usage = usage;

    // Not specified yet.
    this->specified = false;

    // If the option expects arguments, but no defaults are given...
    if(type != OPTION_TYPE_FLAG) {
        this->required = true;
    } else {
        this->required = false;
    }

    // Do a check to make sure the default arguments are valid
    this->AssertArgsValid();
}


// Checks current arguments (be they default or otherwise) for continuity given option type
void Option::AssertArgsValid(void) {
    switch(this->type) {

        // Flag options expect no arguments
        case OPTION_TYPE_FLAG:
            if(this->arguments.size() != 0) {
                printf("Error, '%s' flag options do not accept arguments.\n", this->longIdent.c_str());
                exit(1);
            } else break;

        // Single options expect a single argument
        case OPTION_TYPE_SINGLE:
            if(this->arguments.size() != 1) {
                printf("Error, '%s' single argument options expect only one argument.\n", this->longIdent.c_str());
                exit(1);
            } else break;

        // List options expect at least one argument
        case OPTION_TYPE_LIST:
            if(this->arguments.size() < 1) {
                printf("Error, '%s' list options expect at least one argument.\n", this->longIdent.c_str());
                exit(1);
            } else break;

        // Argument typeid wasn't recognised, will probs only trip when
        // there is heap corruption (read: highly unlikely) best have a default anyway though...
        default:
            printf("Error, option argument count assert check failure, option typeid not recognised\n");
            exit(1);
    }
}


// Set arguments, ovverides default args and indicates that the option was specified
void Option::SetArguments(vector<string>& args) {

    // Do not allow repeat specification of an argument
    if(this->specified) {
        printf("Error, option '%s' already specified.\n", this->longIdent.c_str());
        exit(1);
    }

    // Set the args and indicate that the option has been specified
    this->arguments = args;
    this->specified = true;

    // Do a check to make sure the arguments are valid
    this->AssertArgsValid();
}


// Get arguments for a command line option
vector<string>& Option::GetArguments(void) {
    return this->arguments;
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
bool Option::OptMatches(string& opt) {
    if(opt == this->longIdent) {
        return true;
    } else {
        return false;
    }
}


//========[OPTION PARSER SUBCLASS]===============================================================//


// Constructor clear options (and add a help option)
OptionParser::OptionParser(void) {

    // Clear any present options
    this->options.clear();

    // Add the help option
    this->AddOption(OPTION_TYPE_FLAG, "help", 'h',
                    "Displays help message.");
}


// Instantiate and add an option
void OptionParser::AddOption(ArgType_t type, string longIdent, char shortIdent, string usage, vector<string> def) {
    this->options.push_back(Option(type, longIdent, shortIdent, usage, def));
}


// Instantiate and add an option with no arguments
void OptionParser::AddOption(ArgType_t type, string longIdent, char shortIdent, string usage) {
    this->options.push_back(Option(type, longIdent, shortIdent, usage));
}


// Parse a single short option with arguments
void OptionParser::ParseShortOption(char opt, vector<string> optArgs) {
    printf("Short option '%c' with %lu arguments.\n", opt, optArgs.size());

    // Iterate over options looking for matches, applies only to the first match
    this->GetOption(opt).SetArguments(optArgs);
}


// Short options may appear as a series of flag options
void OptionParser::ParseShortOptionBlock(string& opt, vector<string>& optArgs) {

    // Only continue if the option is valid
    if(!ValidShortOption(opt)) {
        printf("Error, '%s' is not a valid command line option specifier.\n", opt.c_str());
        exit(1);
    }

    // Handle each short option separately, passing args to the last option only
    string optStripped = StripLeadingDashes(opt);
    for(unsigned i = 0; i < optStripped.size(); i++) {
        if(i == optStripped.size() - 1) {
            this->ParseShortOption(optStripped[i], optArgs);
        } else {
            this->ParseShortOption(optStripped[i], {});
        }
    }
}


// Parse a long option, long options always appear alone
void OptionParser::ParseLongOption(string& opt, vector<string>& optArgs) {
    printf("Long option '%s' with %lu arguments.\n", opt.c_str(), optArgs.size());

    // Only continue of the option is valid
    if(!ValidLongOption(opt)) {
        printf("Error, '%s' is not a valid command line option specifier.\n", opt.c_str());
        exit(1);
    }

    // Iterate over all options
    string optStripped = StripLeadingDashes(opt);
    this->GetOption(optStripped).SetArguments(optArgs);
}


// Get arguments for a given option
// Returns a vector of strings with the option at the beginning and arguments following
vector<string> OptionParser::GetOptArgs(vector<string>& argv, unsigned optIndex) {
    vector<string> optArgs;

    // Check that the option is a valid option string
    if(argv[optIndex][0] != '-') {
        printf("Error, '%s' is not a valid command line option.\n", argv[optIndex].c_str());
        exit(1);
    }

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


// Parse an array of command line options
void OptionParser::Parse(vector<string>& argv) {

    // Iterate over options
    for(unsigned i = 0; i < argv.size();) {

        // Get option and arguments
        string opt = argv[i];
        vector<string> optArgs = this->GetOptArgs(argv, i);
        i += optArgs.size() + 1;

        // Parse as long or short option apropriately
        if(CountLeadingDashes(opt) == 2) {
            this->ParseLongOption(opt, optArgs);
        } else if(CountLeadingDashes(opt) == 1) {
            this->ParseShortOptionBlock(opt, optArgs);
        } else {
            printf("Error, '%s' is not a valid command line option specifier.\n", opt.c_str());
            exit(1);
        }
    }
}


// Packs command line arguments into an stl vector
void OptionParser::Parse(int argc, char **argv) {

    // Pack into stl vector
    vector<string> args;
    for(int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Parse the vector
    this->Parse(args);
}


// Get option with a given name
Option& OptionParser::GetOption(string longIdent) {

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
Option& OptionParser::GetOption(char shortIdent) {

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


// Get option arguments
vector<string>& OptionParser::GetOptionArguments(string longIdent) {
    return this->GetOption(longIdent).GetArguments();
}

vector<string>& OptionParser::GetOptionArguments(char shortIdent) {
    return this->GetOption(shortIdent).GetArguments();
}


//
