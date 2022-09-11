#include "format.h"

#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours = (seconds / 60) / 60;

  int minutes = (seconds / 60);
  
  int finalSeconds = seconds % 60;  

  if (minutes > 60) {
    minutes = minutes % 60;
  } else {
    hours = 0;
  }

  return to_string(hours) + ":" + to_string(minutes) + ":" +
         to_string(finalSeconds);
}