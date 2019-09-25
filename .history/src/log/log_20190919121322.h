#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdexcept>
#include <string>

namespace log
{
	[[noreturn]] void exit()
	{
		std::exit(0);
	}

}

#endif // LOG_H