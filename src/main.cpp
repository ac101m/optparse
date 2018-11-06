#include "stdio.h"
#include "unistd.h"
#include <iostream>


#include "options.hpp"


// Test the options parser
int main(int argc, char **argv) {

  // Add test options to the parser
  OptionParser options(argc, argv);

  // Test argument 1, just for setting the print message
  options.Add(Option(
    "message", 'm', ARG_TYPE_STRING,
    "Sets the message to display to the terminal.",
    {"Hello world"}));

  // Test argument 2, prints the message "cake"
  options.Add(Option(
    "count", 'n', ARG_TYPE_INT,
    "Sets the number of times the argument is displayed.",
    {"1"}));

  // Get and convert options
  int count = options.Get("count");
  string message = options.Get("message");

  // Print the message n times
  for(int i = 0; i < count; i++) {
    cout << message << "\n";
  }

  // El fin
  return 0;
}
