#pragma once
#include "RenderCoreDefine.h"
#include "gui/Gui.h"
#include "camera/Camera.h"
#include <memory>

RENDER_CORE_BEGIN

class CameraPanel final : public Gui 
{
public:
	CameraPanel():
		m_weak_ptr_camera_{}
	{

	}

	void SetCamera(std::shared_ptr<Camera> ptr_camera)
	{
		m_weak_ptr_camera_ =  ptr_camera ;
	}

	void Draw() override;

private:
	std::weak_ptr<Camera> m_weak_ptr_camera_;

};

RENDER_CORE_END
