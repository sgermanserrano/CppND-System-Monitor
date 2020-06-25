#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
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
  filestream.close();
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string aux;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> aux >>  kernel;
  }
  stream.close();
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  float mem_total;
  float mem_free;
  float mem_available;
  float buffers;
  string line;
  string aux;
  string mem_value_str;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {

    // Get the 4 first lines 
    for (int i = 0; i < 3; i++)
    {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      switch (i)
      {
      case 0:
        linestream >> aux >> mem_value_str;
        mem_total = stof(mem_value_str);
        break;

      case 1:
        linestream >> aux >> mem_value_str;
        mem_free = stof(mem_value_str);
        break;

      case 2:
        linestream >> aux >> mem_value_str;
        mem_available = stof(mem_value_str);
        break;

      case 3:
        linestream >> aux >> mem_value_str;
        buffers = stof(mem_value_str);
        break;
      }
    }
  }

  stream.close();
  return 1.0 - mem_available/mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  
  long uptime;
  string line;
  string uptime_str;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str;
    uptime = stoi(uptime_str);
  }
  
  stream.close();      
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 

  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {

  string line;
  string value; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  int counter = 1;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long total_time;

  if (stream.is_open())
  {

    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> value)
    {
      
      switch (counter)
      {
      case 14:
        utime = std::stoi(value);
        break;
      case 15:
        stime = std::stoi(value);
        break;
      case 16:
        cutime = std::stoi(value);
        break;
      case 17:
        cstime = std::stoi(value);
        break;
      default:
        break;
      }

      counter += 1;
    }
  }

  stream.close();
  total_time = utime + stime + cutime + cstime;
  return total_time;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 

  string user_str;
  string nice_str;
  string system_str;
  string idle_str;
  string iowait_str;
  string irq_str;
  string softirq_str;
  string steal_str;
  string guest_str;
  string guest_nice_str;
  long user;
  long nice;
  long system;
  long irq;
  long softirq;
  long steal;
  long guest;
  long guest_nice;
  long active_jiffies;

  string line;
  string aux;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> aux >> user_str >> nice_str >> system_str >> idle_str >> 
        iowait_str >> irq_str >> softirq_str >> steal_str >> guest_str >> 
        guest_nice_str;
  }

  stream.close();

  user = std::stoi(user_str);
  nice = std::stoi(nice_str);
  system = std::stoi(system_str);
  irq = std::stoi(irq_str);
  softirq = std::stoi(softirq_str);
  steal = std::stoi(steal_str);
  guest = std::stoi(guest_str);
  guest_nice = std::stoi(guest_nice_str);

  active_jiffies = user + nice + system + irq + softirq + steal;
  return active_jiffies; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  string user_str;
  string nice_str;
  string system_str;
  string idle_str;
  string iowait_str;
  string irq_str;
  string softirq_str;
  string steal_str;
  string guest_str;
  string guest_nice_str;
  long idle, iowait;
  long idle_jiffies;

  string line;
  string aux;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> aux >> user_str >> nice_str >> system_str >> idle_str >> 
        iowait_str >> irq_str >> softirq_str >> steal_str >> guest_str >> 
        guest_nice_str;
  }

  stream.close();

  idle = std::stoi(idle_str);
  iowait = std::stoi(iowait_str);

  idle_jiffies = idle + iowait;
  return idle_jiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  
  string line;
  string key;
  string value; 
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes"){
        stream.close();
        return std::stoi(value);
      }
    }
  }
  
  stream.close();
  // Return 0 if stream not open or loop completes
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {

  string line;
  string key;
  string value; 
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running"){
        stream.close();
        return std::stoi(value);
      }
    }
  }

  stream.close();
  // Return 0 if stream not open or loop completes
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {

  string line;
  string command{""}; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    stream.close();
    return command;
  }

  stream.close();
  // Return empty string if command not found
  return command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 

  string line;
  string key;
  string value; 
  string ram_str{""};
  string aux; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  long ram;

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> ram_str;
      if (key == "VmSize"){
        stream.close();
        ram = std::stoi(ram_str);

        // Return Mb
        return std::to_string(ram/1000);
      }
    }
  }

  stream.close();
  // Return empty string if uid not found
  return ram_str; 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

  string line;
  string key;
  string uid{""};
  string aux; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid"){
        stream.close();
        return uid;
      }
    }
  }

  stream.close();
  // Return empty string if uid not found
  return uid; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 

  string line;
  string key{""};
  string id;
  string uid;
  string aux; 

  std::ifstream stream(kPasswordPath);
  uid = LinuxParser::Uid(pid);

  if (stream.is_open())
  { 
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> aux >> id;
      if (id == uid){
        stream.close();
        return key;
      }
    }
  }
  
  stream.close();
  // Return empty string if user not found
  return key; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 

  string line;
  string value; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  int counter = 1;
  long starttime;
  long seconds;

  if (stream.is_open())
  {

    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> value)
    {
      
      switch (counter)
      {
      case 22:
        starttime = std::stoi(value);
        break;
      default:
        break;
      }

      counter += 1;
    }
  }

  stream.close();
  seconds = LinuxParser::UpTime() - starttime/sysconf(_SC_CLK_TCK);
  return seconds; 
}