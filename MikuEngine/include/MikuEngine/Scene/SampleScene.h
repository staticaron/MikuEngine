#pragma once

#include "AppLevelStuff.h"
#include "entt/entt.hpp"

#include "Core.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class MIKU_API SampleScene : public Scene
	{
	public:
		SampleScene();

		void Update( double dt ) override;
		void Render( AppLevelStuff& appLevelStuff ) const override;
		void RenderImGui() override;

	private:
	private:
		entt::registry m_Registry;
	};
}
