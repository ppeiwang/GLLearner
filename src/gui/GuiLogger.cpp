#include "gui/GuiLogger.h"
#include "imgui/include/imgui.h"

bool operator&(int l, GUILoggerLevel r)
{
	return l & static_cast<int>(r);
}

ImVec4 GetLogLevelColor(GUILoggerLevel lvl)
{
	ImVec4 color = { 1.0f, 0.96f, 0.88f, 1.0f };
	switch (lvl)
	{
	case GUILoggerLevel::E_Info:
		break;
	case GUILoggerLevel::E_Warning:
		color = ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	case GUILoggerLevel::E_Error:
		color = ImVec4{ 1.0f, 0.15f, 0.15f, 1.0f };
		break;
	default:
		break;
	}
	return color;
}

GuiLogger::GuiLogger()
	: m_update_scrolling_bar_(false)
{
	memset(&m_filter_[0], 1, sizeof(m_filter_));
}

void GuiLogger::Draw()
{
	ImGui::Begin("Log");
	if (ImGui::Button("Clear"))
	{
		Clear();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Info", &m_filter_[static_cast<size_t>(GUILoggerLevel::E_Info)]);
	ImGui::SameLine();
	ImGui::Checkbox("Warning", &m_filter_[static_cast<size_t>(GUILoggerLevel::E_Warning)]);
	ImGui::SameLine();
	ImGui::Checkbox("Error", &m_filter_[static_cast<size_t>(GUILoggerLevel::E_Error)]);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2{ 0,0 }, false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetWindowWidth() * 0.9f);

	auto itrHead = m_buffer_.Head();
	auto itrTail = m_buffer_.Tail();
	while (itrHead != m_buffer_.End())
	{
		if (static_cast<size_t>(itrHead->second) < m_filter_.size() && m_filter_[static_cast<size_t>(itrHead->second)])
		{
			ImGui::TextColored(GetLogLevelColor(itrHead->second), "%s", itrHead->first.c_str());
		}

		if (itrHead == itrTail) break;

		++itrHead;

		if (itrHead == m_buffer_.End())
		{
			itrHead = m_buffer_.Begin();
		}
	}

	if (m_update_scrolling_bar_)
	{
		ImGui::SetScrollHere(1.0f);
	}
	m_update_scrolling_bar_ = false;
	ImGui::EndChild();
	ImGui::End();
}

void GuiLogger::Push(const std::string& msg, GUILoggerLevel level)
{
	m_buffer_.Push(std::make_pair(msg, level));
	m_update_scrolling_bar_ = true;
}

void GuiLogger::Clear()
{
	m_buffer_.Clear();
}

void GuiLogger::SetFilter(int flag)
{
	m_filter_[static_cast<size_t>(GUILoggerLevel::E_Info)] = flag & GUILoggerLevel::E_Info;
	m_filter_[static_cast<size_t>(GUILoggerLevel::E_Warning)] = flag & GUILoggerLevel::E_Warning;
	m_filter_[static_cast<size_t>(GUILoggerLevel::E_Error)] = flag & GUILoggerLevel::E_Error;
}

void GuiLogger::Error(const std::string& msg)
{
	Push(msg, GUILoggerLevel::E_Error);
}

void GuiLogger::Warning(const std::string& msg)
{
	Push(msg, GUILoggerLevel::E_Warning);
}

void GuiLogger::Info(const std::string& msg)
{
	Push(msg, GUILoggerLevel::E_Info);
}

