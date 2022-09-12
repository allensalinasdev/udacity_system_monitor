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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long uptime = LinuxParser::UpTime();

  vector<string> values = LinuxParser::ReadStatFile(pid_);

  long utime = stol(values[13]);

  long stime = stol(values[14]);
  long cutime = stol(values[15]);
  long cstime = stol(values[16]);
  long starttime = stol(values[21]);

  long total_time = utime + stime;

  if (total_time > 0) {
    //   total_time = total_time + cutime + cstime;

    long hertz = sysconf(_SC_CLK_TCK);
        // std::cout << "CONTENIDO para pid " << to_string(pid_) << ":"
        //     << to_string(values.size()) << " " << values[13] << "\n";
    float seconds = uptime - (starttime / hertz);

        // std::cout << "Uptime: " << to_string(uptime) <<  "starttime: " << to_string(starttime) <<" HERTZ: " << to_string(hertz) <<" Seconds: "<< to_string(seconds) << "\n";

    long cpu_usage = (total_time / hertz) / seconds;

    return cpu_usage;
  }

  return 0;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() {
  string uid = LinuxParser::Uid(pid_);
  std::string user = LinuxParser::User(uid);
  return user;
}

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process &a) { 
    // return pid_ > a.pid_; 
        return CpuUtilization() > a.CpuUtilization(); 
    }