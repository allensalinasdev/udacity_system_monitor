#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using namespace std;

// Returns the system's CPU
Processor &System::Cpu() {
  return cpu_;
}

bool SortFunction(Process &p1, Process &p2) { return p1 < p2; }

// Returns a container composed of the system's processes
vector<Process> &System::Processes() {
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();

  for (int pid : pids) {
    processes_.emplace_back(pid);
  }

  std::sort(processes_.begin(), processes_.end(),
            [](Process &p1, Process &p2) { return p1 < p2; });

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }