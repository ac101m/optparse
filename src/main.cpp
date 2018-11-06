#include "stdio.h"
#include "unistd.h"
#include <iostream>

#include "options.hpp"


// Test the options parser
int main(int argc, char **argv) {

  // Add test options to the parser
  OptionParser options(argc, argv);

  // Test argument 1, just for setting the print message
  options.AddOption("message", 'm',                                   // Type and identifiers
                    "Set the message to display to the terminal.",    // Option help message
                    "Hello world");                                   // Default argument(s)

  // Test argument 2, prints the message "cake"
  options.AddOption("cake", 'c',                                      // Identifiers
                    "Prints the message \"cake\" to the terminal.");  // Help message

  // Print out the message passed in as a parameter
  printf("%s\n", options.Get<string>("message")[0].c_str());

  // Test the cake option
  if(options.Specified("cake")) printf("cake!\n");

  // El fin
  return 0;
}
