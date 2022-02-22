#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

/* DONE: Return the aggregate CPU utilization
cpu_values[0] = user
cpu_values[1] = nice
cpu_values[2] = system
cpu_values[3] = idle
cpu_values[4] = iowait
cpu_values[5] = irq
cpu_values[6] = softirq
cpu_values[7] = steal
cpu_values[8] = guest
cpu_values[9] = guest_nice

Total non_idle = user + nice + system + irq + softirq + steal
Total idle = idle + iowait
(according to https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux)
*/

float Processor::Utilization() { 
  std::vector<long> cpu_values = LinuxParser::CpuUtilization(); 
  long non_idle = cpu_values[0] + cpu_values[1] +  cpu_values[2] +  cpu_values[5] +  cpu_values[6] +  cpu_values[7]; //user+nice+system+irq+softirq+steal
  long idle =  cpu_values[3] +  cpu_values[4]; // idle+iowait
  long total = non_idle + idle;

  long prev_total = prev_idle + prev_non_idle;
  
  float non_idle_delta = (float)non_idle - (float)prev_non_idle;
  float total_delta = (float)total - (float)prev_total;
  
  prev_idle = idle;
  prev_non_idle = non_idle;

  return non_idle_delta / total_delta; 
}