#include "stdio.h"
#include "unistd.h"
#include <iostream>
using namespace std;


#include "optparse.hpp"


// Build option parser
OptionParser BuildOptionParser(int argc, char **argv) {

  // Description message
  stringstream ss;
  ss << "Simple test program for this option parsing library.\n";
  ss << "Does some printing and other things.";

  // Add test options to the parser
  OptionParser options(argc, argv, ss.str());

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
    {"3.14159"}
  ));

  // Return option parser
  return options;
}


// Test the options parser
int main(int argc, char **argv) {

  // Option parser
  OptionParser options = BuildOptionParser(argc, argv);

  // Get and convert options
  int count = options.Get("count");
  string message = options.Get("message");
  bool thereIsCake = options.Get("cake");
  double dpi = options.Get("pi");
  float fpi = options.Get("pi");

  // Print the message n times
  for(int i = 0; i < count; i++) cout << message << "\n";

  // Print the word cake if apropriate
  if(thereIsCake) cout << "cake\n";

  // Print PI from the command line option
  cout << "Pi (double) is: " << dpi << "\n";
  cout << "Pi (float) is: " << fpi << "\n";

  // El fin
  return 0;
}
