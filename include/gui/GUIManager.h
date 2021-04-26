#pragma once
#include "RenderCoreDefine.h"
#include "utilities/Singleton.h"
#include "gui/Gui.h"
#include <memory>
#include <list>
 
struct GLFWwindow;

RENDER_CORE_BEGIN

class GUIManager : public Singleton<GUIManager>
{
public:
	void Init(GLFWwindow* window);

	void Update();

	void DeInit();

	void AddGuiPanel(std::shared_ptr<Gui> ptr_gui);

private:
	void Draw();
	
private:
	static GUIManager* m_ptr_instance_;

	std::list<std::weak_ptr<Gui>> m_panel_children_list_;

	bool show_demo_window = true;
};

RENDER_CORE_END
