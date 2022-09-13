#include "format.h"

#include <string>

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
  int hours = (seconds / 60) / 60;

  int minutes = (seconds / 60);

  int finalSeconds = seconds % 60;

  if (minutes > 60) {
    minutes = minutes % 60;
  } else {
    hours = 0;
  }

  return Format::FormatNumber(to_string(hours)) + ":" + Format::FormatNumber(to_string(minutes)) + ":" +
         Format::FormatNumber(to_string(finalSeconds));
}

string Format::FormatNumber(std::string number) {
  if (number.size() == 1) {
    number = "0" + number;
  }

  return number;
}