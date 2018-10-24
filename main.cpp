#include "stdio.h"
#include "unistd.h"
#include <iostream>

#include "options.hpp"


// Test the options parser
int main(int argc, char **argv) {

    // Add test options to the parser
    OptionParser parser;

    // Test argument 1, just for setting the hello world message
    parser.AddOption(OPTION_TYPE_SINGLE, "message", 'm',                // Type and identifiers
                     "Set the message to display to the terminal.",     // Option help message
                     {"Hello world"});                                  // Default arguments

    // Run the parser
    parser.Parse(argc, argv);

    // Print out the message passed in as a parameter
    printf("%s\n", "Placeholder message");

    // El fin
    return 0;
}
