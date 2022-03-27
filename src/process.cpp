#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

#include "iostream"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    // vector<string> jiffies = LinuxParser::CpuUtilization();

    // float total = 0; //Time in jiffies
    // float workers = 0;

    // for (int i = 0; i < jiffies.size();i++){
    //     total+=stof(jiffies[i]);
    //     if(i<3){
    //         workers += stof(jiffies[i]);
    //     }
    // }

    // // for (string value: jiffies)
    // // {
    // //     total+=stof(value);
    // // }

    // //float work = stof(jiffies[0]) + stof(jiffies[1]) + stof(jiffies[2]);

    // float percentage = workers/total*100;

    // //std::cout<<"Workers: " << workers << ", total: " << total << "% << "<< percentage <<"\n"; 

    // // float seconds = workers / total / sysconf(_SC_CLK_TCK);
    // float seconds = workers / sysconf(_SC_CLK_TCK);

    // // float seconds = total / sysconf(_SC_CLK_TCK);

    // //std::cout << "AYSJ CpuUtilization() seconds: " << seconds << "\n";
    // return seconds;

    return 0;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() {
    string uid = LinuxParser::Uid(pid_);
    std::string user = LinuxParser::User(uid); 
    return user; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }