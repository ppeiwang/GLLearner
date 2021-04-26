#include "logger/Logger.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include <iomanip>

RENDER_CORE_BEGIN

void Logger::Info(const std::string& s)
{
	std::cout << s << std::endl;
	auto ptr_gui_logger = GetInstance().m_weak_ptr_logger_.lock();
	if (ptr_gui_logger)
	{
		ptr_gui_logger->Info(s);
	}
}

void Logger::Warning(const std::string& s)
{
	std::cout << s << std::endl;
	auto ptr_gui_logger = GetInstance().m_weak_ptr_logger_.lock();
	if (ptr_gui_logger)
	{
		ptr_gui_logger->Warning(s);
	}
}

void Logger::Error(const std::string& s)
{
	std::cerr << s << std::endl;
	auto ptr_gui_logger = GetInstance().m_weak_ptr_logger_.lock();
	if (ptr_gui_logger)
	{
		ptr_gui_logger->Error(s);
	}
	assert(false);
}

void Logger::SetGuiLogger(std::shared_ptr<GuiLogger> ptr_logger)
{
	m_weak_ptr_logger_ = ptr_logger;
}

std::string Logger::ToString(const glm::vec3& v)
{
	std::stringstream ss;	
	ss << "{x:" << v.x << ", y:" << v.y << ", z:" << v.z << " }" ;
	return ss.str();
}

std::string Logger::ToString(const glm::quat& q)
{
	std::stringstream ss;	
	ss << "{w:" << q.w <<  q.x << ", x:" << q.x << ", y:" << q.y << ", z:" << q.z << " }" ;
	return ss.str();
}

RENDER_CORE_END
