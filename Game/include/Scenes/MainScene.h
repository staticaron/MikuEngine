#pragma once

#include "AppLevelStuff.h"
#include "entt/entt.hpp"

#include "GameObjects/Camera.h"
#include "MikuEngine/Scene/Scene.h"

namespace RhythmGame
{
	class MainScene : public MikuEngine::Scene
	{
	public:
		MainScene();
		~MainScene();

		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImGui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;

	private:
		entt::registry m_Registry;

		MikuEngine::Camera m_Camera;
	};
}
