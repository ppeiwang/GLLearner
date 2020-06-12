#include "CameraPanel.h"
#include "imgui/include/imgui.h"
#include <string>

void CameraPanel::Draw()
{
	std::shared_ptr<Camera> ptr_camera = m_weak_ptr_camera_.lock();
	if (ptr_camera)
	{
		ImGui::Begin("Camera");
	
		const auto& position = ptr_camera->GetPosition();
		const auto& rotation = ptr_camera->GetRotation();
		const auto& euler = ptr_camera->GetEuler();

		ImGui::Text("%s", "Position:");
		ImGui::Indent();
		std::string buffer;
		buffer = "x = " + std::to_string(position.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(position.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(position.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();

		ImGui::Text("%s", "Rotation:");
		ImGui::Indent();
		buffer = "x = " + std::to_string(rotation.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(rotation.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(rotation.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();

		ImGui::Text("%s", "Rotation(Euler):");
		ImGui::Indent();
		buffer = "x = " + std::to_string(euler.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(euler.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(euler.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();

		ImGui::Separator();
		
		if (ImGui::Button("Reset Camera"))
		{
			ptr_camera->Reset();
		}

		ImGui::End();	
	}
}
