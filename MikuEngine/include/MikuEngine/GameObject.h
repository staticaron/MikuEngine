#pragma once

#include "Components/SpriteRenderer.h"
#include "GameObjects/Camera.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class GameObject
	{
	public:
		void Render( const Renderer& renderer, const TextureManager& textureManager, const Camera& camera ) const;
		void RenderImGui() {};

	private:
	private:
		SpriteRenderer m_SpriteRenderer;
	};
}
