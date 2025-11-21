#pragma once

#include "Components/SpriteRenderer.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class GameObject
	{
	public:
		void Render( const Renderer& renderer ) const;
		void RenderImGui() {};

	private:
	private:
		SpriteRenderer m_SpriteRenderer;
	};
}
