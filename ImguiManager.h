#pragma once

struct GLFWwindow;

class ImguiManager
{
private:
	ImguiManager() = default;

	~ImguiManager();

public:
	static ImguiManager& GetInstance();

	void Init(GLFWwindow* window);

	void Update();

	void DeInit();
	
private:
	static ImguiManager* m_ptr_instance_;

	bool show_demo_window = true;
};

