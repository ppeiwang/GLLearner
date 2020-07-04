#pragma once
#include <memory>
#include <string>
#include "Camera.h"
#include "glm/glm.hpp"

class Scene
{
public:
	Scene(const std::string&);

	void CreateCamera();

	std::shared_ptr<Camera> GetCamera();

	void SetLightPosition(const glm::vec3& pos);

	const glm::vec3& GetLightPosition() const;

private:
	std::string m_str_scene_name_;

	std::shared_ptr<Camera> m_ptr_camera_instance_;

	glm::vec3 m_vec3_light_pos_;

};