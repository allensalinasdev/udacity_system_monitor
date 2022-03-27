#include "processor.h"
#include "linux_parser.h"
#include "iostream"

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    // float aggregateUtilization = 0;
    // vector<string> jiffies = LinuxParser::CpuUtilization();
    // for (string value: jiffies)
    // {
    //     aggregateUtilization += stof(value);
    // }
    
    // return aggregateUtilization;

//////////////////////////////////////////////////////////

vector<string> jiffies = LinuxParser::CpuUtilization();

    float total = 0; //Time in jiffies
    float workers = 0;

    for (int i = 0; i<jiffies.size();i++){
        total+=stof(jiffies[i]);
        if(i<3){
            workers += stof(jiffies[i]);
        }
    }

    // for (string value: jiffies)
    // {
    //     total+=stof(value);
    // }

    //float work = stof(jiffies[0]) + stof(jiffies[1]) + stof(jiffies[2]);

    float percentage = workers/total;

    //std::cout<<"Workers: " << workers << ", total: " << total << " Perc: << "<< percentage <<"\n"; 

    // float seconds = workers / total / sysconf(_SC_CLK_TCK);
    // float seconds = total / sysconf(_SC_CLK_TCK);

    //std::cout << "AYSJ CpuUtilization() seconds: " << seconds << "\n";
    return percentage;

 }