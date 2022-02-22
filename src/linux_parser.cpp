#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

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
  string os, kernel, version;
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
  string line;
  string key;
  string value;

  int mem_total = 0;
  int mem_free = 0;

  float percentage = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = std::stoi(value);
        }
        else if (key == "MemFree:") {
          mem_free = std::stoi(value);
        }  
      }
    }
  }
  // check whether mem_total is not 0 to avoid div by 0
  if (mem_total != 0) percentage = (float)(mem_free)/(float)(mem_total);
  
  return percentage;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string time1 = "";
  string time2 = "";

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    stream >> time1 >> time2;
  }
  
  return std::stol(time1);
}

// NOT NEEDED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// NOT NEEDED: Read and return the number of active jiffies for a PID
// Jiffies not needed anymore when kernel >= 2.6
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// NOT NEEDED: Read and return the number of active jiffies for the system
// Jiffies not needed anymore when kernel >= 2.6
long LinuxParser::ActiveJiffies() { return 0; }

// NOT NEEDED: Read and return the number of idle jiffies for the system
// Jiffies not needed anymore when kernel >= 2.6
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
// return value changed to long for easier handling in processor class
vector<long> LinuxParser::CpuUtilization() { 
  vector<long> cpu_values;  
  string value = "";
  
  
  std::ifstream stream (kProcDirectory + kStatFilename);
  stream >> value;
  if (value == "cpu") {
    for (int i = 0; i < 10; i++) {
      stream >> value;
      cpu_values.push_back(stol(value));
    }
  }
  return cpu_values; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //search for total number of processes, separated by " ";
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //search for total number of running processes, separated by " ";
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string cmd = "";

  if (stream.is_open()) {
    stream >> cmd;
  }
  return cmd;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line = "";
  string key = "";
  int value = 0;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //search for key "VmSize" to get Ram usage
        if (key == "VmSize:") {
          //convert to MB by dividing by 1024
          return to_string((int)value/1024);
        }
      }
    }
  }
  return ""; 
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line = "";
  string key = "";
  string value = "";
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //search for key "Uid" to get user ID
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "";
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream filestream(kPasswordPath);
  string line = "";
  string key = "";
  string user = "";
  string dump = "";

  string uid = Uid(pid);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //replace colons with spaces
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> dump >> key) {

        //search for user id to get user name
        if (key == uid) {
          return user;
        }
      }
    }
  }
  return "User not found";
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string starttime = "";
  int i = 1;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (i++ < 23) {
      stream >> starttime; //22nd value needed -> clockticks after system boot
    }
  }
  //convert clockticks to seconds, subtract from system uptime
  return UpTime() - (long)(std::stol(starttime) / sysconf(_SC_CLK_TCK));
  
}


// Read and return CPU utilization for each process (pid)
// return value changed to long for easier handling in processor class
// calculation: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
vector<long> LinuxParser::CpuUtilization(int pid) { 
  vector<long> cpu_values;
  string value = "";  
  int i = 1;
  
  std::ifstream stream (kProcDirectory + to_string(pid) + kStatFilename);
  //break loop after 22nd item, only 14-22 needed
  while (i < 23) {
    stream >> value;
    //values needed #14-utime, #15-stime, # 16-cutime, #17-cstime, #22-starttime
    if (i == 14 || i == 15 || i == 16 || i == 17 || i == 22) {
      cpu_values.push_back(stol(value));
    }
    i++;
  }

  return cpu_values;
 
}
