#pragma once
#include "RenderCoreDefine.h"
#include "utilities/Singleton.h"
#include "gui/GuiLogger.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <memory>
#include <string>

RENDER_CORE_BEGIN

class Logger : public Singleton<Logger>
{
	friend Singleton<Logger>;
public:
	static void Info(const std::string& s);

	static void Warning(const std::string& s);

	static void Error(const std::string& s);

	void SetGuiLogger(std::shared_ptr<GuiLogger> ptr_logger);

private:
	std::weak_ptr<GuiLogger> m_weak_ptr_logger_;

public:
	static std::string ToString(const glm::vec3& v);
	
	static std::string ToString(const glm::quat& q);
};

RENDER_CORE_END
