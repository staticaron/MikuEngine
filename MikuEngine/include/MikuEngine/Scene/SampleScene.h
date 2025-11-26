#pragma once

#include "entt/entt.hpp"

#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class SampleScene : public Scene
	{
	public:
		SampleScene();

		void Update( double dt ) override;
		void Render( const Renderer& renderer, const TextureManager& textureManager ) const override;
		void RenderImGui() override;

	private:
	private:
		entt::registry m_Registry;
	};
}
