#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float aggregateUtilization = 0;
    vector<string> jiffies = LinuxParser::CpuUtilization();
    for (string value: jiffies)
    {
        aggregateUtilization += stof(value);
    }
    
    return aggregateUtilization;
 }