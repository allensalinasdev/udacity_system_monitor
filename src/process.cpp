#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "iostream"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

//Based on: https://www.baeldung.com/linux/total-process-cpu-usage
float Process::CpuUtilization() {
  long system_uptime = LinuxParser::UpTime();

  vector<string> values = LinuxParser::ReadStatFile(pid_);

  float utime = stof(values[13]);
  float stime = stof(values[14]);
  float starttime = stof(values[21]);

  float clock_ticks = sysconf(_SC_CLK_TCK);

  float utime_sec = utime / clock_ticks;
  float stime_sec = stime / clock_ticks;
  float starttime_sec = starttime / clock_ticks;

  float process_elapsed_sec = system_uptime - starttime_sec;
  float process_usage_sec = utime_sec + stime_sec;
  float process_usage = process_usage_sec / process_elapsed_sec;

  return process_usage;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() {
  string uid = LinuxParser::Uid(pid_);
  std::string user = LinuxParser::User(uid);
  return user;
}

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process &a) { return pid_ > a.pid_; }