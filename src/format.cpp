#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 

  int h, m, s;
  long remainder;
  string time_str;
  
  h = seconds / 3600;
  remainder = seconds - h*3600;
  m = remainder / 60;
  remainder -= m*60;
  s = remainder;

  time_str = h < 10 ? string("0") + std::to_string(h) : std::to_string(h);
  time_str += string(":");
  time_str += m < 10 ? string("0") + std::to_string(m) : std::to_string(m);
  time_str += string(":");
  time_str += s < 10 ? string("0") + std::to_string(s) : std::to_string(s);

  return time_str; 

  }