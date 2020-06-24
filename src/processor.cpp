#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  return (float(LinuxParser::Jiffies()) - LinuxParser::IdleJiffies()) / LinuxParser::Jiffies();
}