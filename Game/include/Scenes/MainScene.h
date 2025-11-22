#pragma once

#include <vector>

#include "GameObjects/Camera.h"
#include "Managers/TextureManager.h"
#include "MikuEngine/GameObject.h"
#include "MikuEngine/Scene/Scene.h"

namespace RhythmGame
{
	class MainScene : public MikuEngine::Scene
	{
	public:
		MainScene();
		~MainScene();

		void Update( double dt ) override;
		void Render( const MikuEngine::Renderer& renderer, const MikuEngine::TextureManager& textureManager ) const override;
		void RenderImGui() override;

	private:
		std::vector<MikuEngine::GameObject> m_GOs;

		MikuEngine::Camera m_Camera;
	};
}
