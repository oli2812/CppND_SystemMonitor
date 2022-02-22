#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid) {
    pid_ = pid;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
// defined as const for usage in operator overload
// calculation based on https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() const { 
  vector<long> cpu_values = LinuxParser::CpuUtilization(pid_); 
  long total_time = 0, elapsed_time = 0; 
  float cpu_usage = 0.0;

  //getting system's frequency/Hertz
  long freq = sysconf(_SC_CLK_TCK);

  //cpu_values #0-utime, #1-stime, #2-cutime, #3-cstime, #4-starttime
  total_time = cpu_values[0] + cpu_values[1] +cpu_values[2] + cpu_values[3];
  elapsed_time = LinuxParser::UpTime() - (long)(cpu_values[4]/freq);

  cpu_usage = ((float)(total_time)/(float)(freq))/(float)(elapsed_time);

  return cpu_usage;  
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return (a.CpuUtilization() < this->CpuUtilization()); }