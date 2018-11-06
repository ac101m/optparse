#ifndef OPTIONS_HPP_INCLUDED
#define OPTIONS_HPP_INCLUDED


#include <vector>
#include <string>
#include <sstream>
using namespace std;



// Class represents a possible command line parameter in the code
class Option {
    private:

        // Initial data
        bool required;
        char shortIdent;
        string longIdent;
        string usage;

        // Data retrieved from command line parameters
        vector<string> args;
        bool specified;

    public:

        // Constructors
        Option(string longIdent, char shortIdent, string usage, vector<string> def);
        bool OptMatches(char opt);
        bool OptMatches(string opt);

        // Parsing options
        void SetArguments(vector<string> args);
        void SetSpecified(void);
        void Parse(vector<string> argv);

        // Get arguments in various forms
        vector<string> GetArguments(void);
        bool Specified(void) {return this->specified;}

        // Templated method for getting converted arguments
        template<typename T>
        vector<T> Get(void) {

            // Check that the command line option actually exists
            if(this->args.size() == 0) {
                printf("Error, '%s' expects at least 1 argument.\n", longIdent.c_str());
                exit(1);
            }

            // Vector of argument type
            vector<T> convertedArgs;

            // Convert all the arguments
            for(unsigned i = 0; i < this->args.size(); i++) {
                stringstream ss(this->args[i]);
                T convertedValue;
                if(ss >> convertedValue) {
                    convertedArgs.push_back(convertedValue);
                } else {
                    printf("Error,'%s' argument '%s' invalid format.\n", longIdent.c_str(), this->args[i].c_str());
                    exit(1);
                }
            }

            // Return the compiled vector of converted arguments
            return convertedArgs;
        }
};



// Parser for command line arguments
class OptionParser {
    private:

        // Raw arguments
        vector<string> argv;

        // Options classes
        vector<Option> options;

    private:

        Option GetOption(string longIdent);
        Option GetOption(char shortIdent);

    public:

        OptionParser(int argc, char **argv);
        void AddOption(string longIdent, char shortIdent, string usage, vector<string> def);
        void AddOption(string longIdent, char shortIdent, string usage, string def);
        void AddOption(string longIdent, char shortIdent, string usage);
        void AddOption(Option opt);

        // Is a given option specified
        bool Specified(string longIdent);

        // Templated function for getting converted arguments
        template<typename T>
        vector<T> Get(string longIdent) {
            return this->GetOption(longIdent).Get<T>();
        }
};


#endif // OPTIONS_HPP_INCLUDED
