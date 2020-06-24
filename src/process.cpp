#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid){
  pid_ = pid;
}

// DONE: Return this process's ID
int Process::Pid() { 
  return pid_; 
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { 
  return 1.*LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(pid_); 
}

// DONE: Return the command that generated this process
string Process::Command() { 
  command_ = LinuxParser::Command(pid_);
  return command_; 
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
  ram_ = LinuxParser::Ram(pid_);
  return ram_; 
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
  user_ = LinuxParser::User(pid_);
  return user_; 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  uptime_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK);
  return uptime_; 
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return a.CpuUtilization() < CpuUtilization(); 
}