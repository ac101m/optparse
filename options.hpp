#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED


#include <vector>
#include <string>
using namespace std;



// Enumerated type for argument type
typedef enum : uint8_t {
    OPTION_TYPE_FLAG,
    OPTION_TYPE_SINGLE,
    OPTION_TYPE_LIST
} ArgType_t;



// Class represents a possible command line parameter in the code
class Option {
    private:

        // Initial data
        bool required;
        ArgType_t type;
        char shortIdent;
        string longIdent;
        string usage;

        // Data retrieved from command line parameters
        vector<string> arguments;
        bool specified;

    private:

        // Internal utility methods
        void AssertArgsValid(void);

    public:

        // Constructors
        Option(ArgType_t type, string longIdent, char shortIdent, string usage, vector<string> def);
        Option(ArgType_t type, string longIdent, char shortIdent, string usage);
        bool OptMatches(char opt);
        bool OptMatches(string& opt);

        // Set arguments
        void SetArguments(vector<string>& args);

        // Get arguments in various forms
        vector<string>& GetArguments(void);
};



// Parser for command line arguments
class OptionParser {
    private:

        vector<Option> options;

    private:

        Option& GetOption(string longIdent);
        Option& GetOption(char shortIdent);
        vector<string>& GetOptionArguments(string longIdent);
        vector<string>& GetOptionArguments(char shortIdent);

    public:

        OptionParser(void);
        void AddOption(ArgType_t type, string longIdent, char shortIdent, string usage, vector<string> def);
        void AddOption(ArgType_t type, string longIdent, char shortIdent, string usage);

        // Command line parameter parsing routines
        vector<string> GetOptArgs(vector<string>& argv, unsigned optIndex);
        void ParseShortOption(char opt, vector<string> optArgs);
        void ParseShortOptionBlock(string& opt, vector<string>& optArgs);
        void ParseLongOption(string& opt, vector<string>& optArgs);
        void Parse(vector<string>& argv);
        void Parse(int argc, char **argv);
};


#endif // OPTIONS_HPP_INCLUDED
