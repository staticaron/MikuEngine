#pragma once

#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class Scene
	{
	public:
		virtual void Update( double dt ) = 0;
		virtual void Render( const Renderer& renderer, const TextureManager& textureManager ) const = 0;
		virtual void RenderImGui() = 0;

	private:
	private:
	};
}
