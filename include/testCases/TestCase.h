#pragma once

namespace RenderCore 
{
	class Scene;
}

namespace Test
{
	class TestCase
	{
	public:
		TestCase() = default;

		virtual ~TestCase() = default;

		virtual void Update()
		{

		}

		virtual void Init()
		{

		}

		virtual void DeInit()
		{

		}

		void SetScene(RenderCore::Scene* ptr)
		{
			m_scene_instance_ = ptr;
		}

	protected:
		RenderCore::Scene* m_scene_instance_{ nullptr };

	};
}
