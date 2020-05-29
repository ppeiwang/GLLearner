#pragma once
#include <string>

class Logger
{
public:
	static void Info(const std::string& s);

	static void Warning(const std::string& s);

	static void Error(const std::string& s);
};
