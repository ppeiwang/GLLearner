#include "Scene.h"

Scene::Scene(const std::string& name):
	m_str_scene_name_{name},
	m_ptr_camera_instance_{nullptr}

{

}

void Scene::CreateCamera()
{
	m_ptr_camera_instance_ = std::make_shared<Camera>();
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	return m_ptr_camera_instance_;
}

void Scene::SetLightPosition(const glm::vec3& pos)
{
	m_vec3_light_pos_ = pos;
}

const glm::vec3& Scene::GetLightPosition() const
{
	return m_vec3_light_pos_;
}

