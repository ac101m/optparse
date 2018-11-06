#include "stdio.h"
#include "unistd.h"
#include <iostream>


#include "options.hpp"


// Build option parser
OptionParser buildOptionParser(int argc, char **argv) {

  // Add test options to the parser
  OptionParser options(argc, argv);

  // String test argument, just for setting the print message
  options.Add(Option(
    "message", 'm', ARG_TYPE_STRING,
    "Sets the message to display to the terminal.",
    {"Hello world"}
  ));

  // Integer test argument, prints the message "cake"
  options.Add(Option(
    "count", 'n', ARG_TYPE_INT,
    "Sets the number of times the argument is displayed.",
    {"1"}
  ));

  // Boolean test argument
  options.Add(Option(
    "cake", 'c', ARG_TYPE_BOOL,
    "Prints the word 'cake' following the previous message.",
    {"false"}
  ));

  // Floating point test argument
  options.Add(Option(
    "pi", 'p', ARG_TYPE_FLOAT,
    "A floating point number to print to the terminal.",
    {"3.14"}
  ));

  // Return option parser
  return options;
}

// Test the options parser
int main(int argc, char **argv) {

  // Get and convert options
  int count = options.Get("count");
  string message = options.Get("message");
  bool thereIsCake = options.Get("cake");
  double pi = options.Get("Pi");

  // Print the message n times
  for(int i = 0; i < count; i++)
    cout << message << "\n";

  // Print the word cake if apropriate
  if(thereIsCake)
    cout << "cake\n";

  // Print PI from the command line option
  cout << options.get(pi) << "\n";

  // El fin
  return 0;
}
