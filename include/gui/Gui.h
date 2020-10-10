#pragma once
#include "RenderCoreDefine.h"

RENDER_CORE_BEGIN

class Gui
{
public:
	Gui() = default;

	~Gui() = default;

	virtual void Draw();

	virtual bool Show();

protected:
	bool m_show_flag_ = true;
};

RENDER_CORE_END
