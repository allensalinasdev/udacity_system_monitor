#include "processor.h"

#include "iostream"
#include "linux_parser.h"

using std::string;
using std::vector;

float Processor::Utilization() {
  vector<string> jiffies = LinuxParser::CpuUtilization();

  float total = 0;  // Time in jiffies
  float workers = 0;

  for (size_t i = 0; i < jiffies.size(); i++) {
    total += stof(jiffies[i]);
    if (i < 3) {
      workers += stof(jiffies[i]);
    }
  }

  float percentage = workers / total;

  return percentage;
}