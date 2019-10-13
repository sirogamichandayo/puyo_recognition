#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdexcept>
#include <string>

#define LOG(msg) std::cout << "[LOG] (" __FILE__ ":" << __LINE__ << ") from " << __PRETTY_FUNCTION__ << "()\n    " << msg << "\n";

#endif // LOG_H