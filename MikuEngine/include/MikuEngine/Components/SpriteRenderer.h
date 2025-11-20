#pragma once

#include "Rendering/Quad.h"
#include "Rendering/Renderer.h"
#include <string>
namespace MikuEngine
{
	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		SpriteRenderer( const std::string& filePath );

		void Render( const Renderer& renderer ) const;
		void Update( double dt );

	private:
	private:
		std::string m_FilePath = RESOURCE_DIR "textures/base.png";

		Quad m_Quad;
	};
}
