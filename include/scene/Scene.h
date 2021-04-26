#pragma once
#include "RenderCoreDefine.h"
#include "camera/Camera.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>

RENDER_CORE_BEGIN

// #todo list:
// 1. IO. in order to save & load a scene file

class Scene
{
public:
	Scene(const std::string&);

	void CreateCamera();

	std::shared_ptr<Camera> GetCamera();

	void SetLightPosition(float x, float y, float z);
	
	void SetLightPosition(const glm::vec3& pos);

	const glm::vec3& GetLightPosition() const;

private:
	std::string m_str_scene_name_;

	std::shared_ptr<Camera> m_ptr_camera_instance_;

	glm::vec3 m_vec3_light_pos_;

};

RENDER_CORE_END