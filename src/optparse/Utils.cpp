#include <string>


unsigned CountLeadingDashes(std::string& opt) {
  unsigned leadingDashes = 0;
  for(unsigned i = 0; i < opt.size(); i++) {
    if(opt[i] == '-') {
      leadingDashes++;
    } else {
      break;
    }
  }
  return leadingDashes;
}


std::string StripLeadingDashes(std::string& opt) {
  std::string r;
  for(unsigned i = 0; i < opt.size(); i++) {
    if(i >= CountLeadingDashes(opt)) {
      r += opt[i];
    }
  }
  return r;
}
