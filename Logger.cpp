#include "Logger.h"
#include <iostream>
#include <assert.h>

void Logger::Info(const std::string& s)
{
	std::cout << s << std::endl;
}

void Logger::Warning(const std::string& s)
{
	std::cout << s << std::endl;
}

void Logger::Error(const std::string& s)
{
	assert(false);
	std::cerr << s << std::endl;
}
