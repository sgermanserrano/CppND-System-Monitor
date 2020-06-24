#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
#include <iostream>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // DONE: See src/process.cpp
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization() const;                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_ = 0;
  std::string command_ = std::string();
  std::string ram_ = std::string();
  std::string uid_ = std::string();
  std::string user_ = std::string();
  long uptime_ = 0;
  float cpu_ = 0.0;
};

#endif