#pragma once

#include "Core.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class MIKU_API Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void Update( double dt ) {};
		virtual void Render( const Renderer& renderer ) const {};
		virtual void RenderImgui() {};

	private:
	};
}
