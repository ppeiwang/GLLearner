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
		const auto& target = ptr_camera->GetTarget();
		const auto& last_position = ptr_camera->last_camera_position;
		const auto& displacement = ptr_camera->camera_displacement;
		const auto& euler = ptr_camera->GetEuler();
		const auto& rotation_matrix = ptr_camera->GetRotationMatrix();

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
		buffer = "w = " + std::to_string(rotation.w);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "x = " + std::to_string(rotation.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(rotation.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(rotation.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();

		ImGui::Text("%s", "Target:");
		ImGui::Indent();
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "x = " + std::to_string(target.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(target.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(target.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();

		ImGui::Text("%s", "Last Position:");
		ImGui::Indent();
		buffer = "x = " + std::to_string(last_position.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(last_position.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(last_position.z);
		ImGui::BulletText("%s", buffer.c_str());
		ImGui::Unindent();


		ImGui::Text("%s", "Displacement:");
		ImGui::Indent();
		buffer = "x = " + std::to_string(displacement.x);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "y = " + std::to_string(displacement.y);
		ImGui::BulletText("%s", buffer.c_str());
		buffer = "z = " + std::to_string(displacement.z);
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

		ImGui::Text("%s", "RotationMatrix");
		ImGui::Columns(3, "RotationMatrix"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("x-axis"); ImGui::NextColumn();
		ImGui::Text("y-axis"); ImGui::NextColumn();
		ImGui::Text("z-axis"); ImGui::NextColumn();
		for (int i = 0; i < 3; i++)
		{
			ImGui::Text("%s", std::to_string(rotation_matrix[0][i]).c_str());ImGui::NextColumn();
			ImGui::Text("%s", std::to_string(rotation_matrix[1][i]).c_str());ImGui::NextColumn();
			ImGui::Text("%s", std::to_string(rotation_matrix[2][i]).c_str());ImGui::NextColumn();
		}
		ImGui::Columns(1);
		
		ImGui::Separator();
		if (ImGui::Button("Reset Camera"))
		{
			ptr_camera->Reset();
		}

		ImGui::End();	
	}
}
