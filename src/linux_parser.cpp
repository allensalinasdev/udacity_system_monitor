#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "iostream"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          filestream.close();
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }

    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;

    stream.close();
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float memTotal, memAvailable;

  string line;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    int lineCount = 0;
    vector<float> values;

    while (lineCount < 10) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      string title, value;
      linestream >> title >> value;
      values.emplace_back(std::stof(value));
      lineCount++;
    }

    stream.close();

    memTotal = values[0];
    memAvailable = values[2];
    float m = (memTotal - memAvailable) / memTotal;

    return m;
  }

  return 0.0;
}

long LinuxParser::UpTime() {
  long uptime;

  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);

    std::string uptimePart = line.substr(0, line.find(" "));

    std::istringstream linestream(uptimePart);

    linestream >> uptime;

    stream.close();
  }

  return uptime;
}

long LinuxParser::Jiffies() {
  vector<string> result = GetJiffies();

  long sum = 0;

  for (string value : result) {
    sum += stol(value);
  }

  return sum;
}

// Reads and returns the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> result = GetJiffies();

  long sum = 0;

  // First three jiffies: user,nice,system
  for (int i = 0; i < 3; i++) {
    sum += stol(result[i]);
  }

  return sum;
}

// Reads and returns the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> result = GetJiffies();

  long sum = stol(result[3]) + stol(result[4]) + stol(result[5]);

  return sum;
}

// Returns all jiffies for the system
vector<string> LinuxParser::CpuUtilization() {
  vector<string> result = GetJiffies();

  return result;
}

// Reads and returns the number of processes for the system
int LinuxParser::TotalProcesses() {
  return std::stoi(
      LinuxParser::FindValueByKey(kProcDirectory + kStatFilename, "processes"));
}

// Reads and returns the number of running processes for the system
int LinuxParser::RunningProcesses() {
  return std::stoi(LinuxParser::FindValueByKey(kProcDirectory + kStatFilename,
                                               "procs_running"));
}

// Reads and returns the command that executed the specified process
string LinuxParser::Command(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kCmdlineFilename;

  string line;

  std::ifstream filestream(filepath);

  if (filestream.is_open()) {
    std::getline(filestream, line);
  }

  return line;
}

// Reads and returns the used RAM for the specified process
string LinuxParser::Ram(int pid) {
  string vmSize = FindValueByKey(
      kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");

  if (vmSize != "") {
    int memory = std::stof(vmSize);
    if (memory > 1024) {
      return to_string(memory / 1024) + " MB";
    } else {
      return to_string(memory) + " KB";
    }
  }

  return "";
}

// Reads and returns the User ID for the specified process
string LinuxParser::Uid(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kStatusFilename;
  return LinuxParser::FindValueByKey(filepath, "Uid:");
}

// Reads and returns the Uptime for the specified process
long LinuxParser::UpTime(int pid) {
  vector<string> clockTicks = ReadStatFile(pid);

  long clockTicksInSeconds = stol(clockTicks[21]) / sysconf(_SC_CLK_TCK);

  return clockTicksInSeconds;
}

std::string LinuxParser::FindValueByKey(std::string filepath, std::string key) {
  string line;
  string lineKey;
  string value = "";
  std::ifstream filestream(filepath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> lineKey >> value) {
        if (lineKey == key) {
          return value;
        }
      }
    }

    filestream.close();
  }

  return value;
}

// Reads and returns all the system jiffies
std::vector<string> LinuxParser::GetJiffies() {
  vector<string> result;

  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  int counter = 1;
  if (stream.is_open()) {
    std::getline(stream, line);
    stream.close();

    std::istringstream ss(line);
    string val;

    while (getline(ss, val, ' ')) {
      if (counter > 2) result.emplace_back(val);

      counter++;
    }
  }

  return result;
}

// Reads and returns the User name the specified process uid
std::string LinuxParser::User(std::string uid) {
  string line;
  string name;
  string x;
  string key;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::stringstream sstream(line);
      std::string tmpValue;
      int value = 0;
      while (std::getline(sstream, tmpValue, ':')) {
        if (value == 0) {
          name = tmpValue;
        } else if (value == 1) {
          x = tmpValue;
        } else if (value == 2) {
          key = tmpValue;
        }
        if (key == uid) {
          return name;
        }
        value++;
      }
    }

    filestream.close();
  }

  return name;
}

std::vector<std::string> LinuxParser::ReadStatFile(int pid) {
  auto path = kProcDirectory + to_string(pid) + kStatFilename;

  vector<string> values;

  string line;
  std::ifstream stream(path);

  if (stream.is_open()) {
    std::getline(stream, line);
    stream.close();

    std::istringstream ss(line);
    string val;

    while (getline(ss, val, ' ')) {
      values.emplace_back(val);
    }
  }

  return values;
}