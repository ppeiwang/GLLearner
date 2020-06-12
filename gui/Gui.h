#pragma once

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
