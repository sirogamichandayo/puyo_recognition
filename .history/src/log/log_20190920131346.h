#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

namespace logger
{
	#define LOG(msg) std::cout << "[LOG] (" __FILE__ ":" << __LINE__ << ") from " << __func__ << "()\n    " << msg << "\n";

	[[noreturn]] void exit()
	{
		std::exit(0);
	}
}

#endif // LOG_H