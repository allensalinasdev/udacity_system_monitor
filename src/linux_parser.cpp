#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "iostream"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal, memFreee;//, memAvailable, buffers;

  string line;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if (stream.is_open()) {
    int lineCount = 0;
    vector<float> values;
    while (lineCount < 2)
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      string title, value;
      linestream >> title >> value;
      values.push_back(std::stof(value));
      lineCount++; 
    }
    memTotal =values[0];
    memFreee = values[1]; 
    float m =  memTotal - memFreee;
// MemTotal:        3982636 kB
// MemFree:          361192 kB
// MemAvailable:    1193020 kB
// Buffers:           17220 kB


    //return 1000;//m;
    return m;
  }
//  return kernel;


  return 0.0; 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
      std::getline(stream, line);

      std::istringstream linestream(line);
      linestream >> uptime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> result = GetJiffies();

  long sum = 0;

  for (string value: result)
  {
    sum += stol(value);
  } 

  return sum; 
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 500; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> result = GetJiffies();

  long sum = 0;

  //First three jiffies: user,nice,system
  for (int i = 0; i < 3; i++)
  {
    sum += stol(result[i]);
  }

  return sum;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> result = GetJiffies();

  long sum = 0;

  //First three jiffies: user,nice,system
  for (int i = 3; i < result.size(); i++)
  {
    sum += stol(result[i]);
  }

  return sum;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> result = GetJiffies();

  return result; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return std::stoi(LinuxParser::FindValueByKey(kProcDirectory + kStatFilename, "processes"));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stoi(LinuxParser::FindValueByKey(kProcDirectory + kStatFilename, "procs_running"));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string filepath = kProcDirectory + to_string(pid) + kCmdlineFilename;

  //std::cout << "AYSJ COmmand filepath: " << filepath << "\n";

  string line;
  
  std::ifstream filestream(filepath);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }

  return line;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kStatusFilename;
  
  string ram = LinuxParser::FindValueByKey(filepath, "VmSize");
  
  if (stof(ram) >= 1024)
  {
    return to_string(stof(ram)/1024) + " MB";
  }else{
    return ram + " KB";
  }
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kStatusFilename;
  return LinuxParser::FindValueByKey(filepath, "Uid");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

std::string LinuxParser::FindValueByKey(std::string filepath, std::string key){
  string line;
  string lineKey;
  string value;
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::cout << "AYSJ TotalProcesses KEY: "<< key << ", VALUE: " << value << "\n";
      std::istringstream linestream(line);
      while (linestream >> lineKey >> value) {
        // std::cout << "AYSJ TotalProcesses KEY: "<< key << ", VALUE: " << value << "\n";
        if (lineKey == key) {
          // std::cout << "AYSJ key found:" << key << " Value: " << value << "\n";
          return value;
        }
      }
    }
  }

  return value;
}

std::vector<string> LinuxParser::GetJiffies(){
  vector<string> result;

  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
      std::getline(stream, line);

      std::istringstream ss(line);
      string val;
      while(getline(ss, val, ' ')) {
        result.push_back(val);
      }
  }

  return result;
}