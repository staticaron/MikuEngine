#pragma once

#include <string>

#include "GameObjects/Camera.h"
#include "Managers/TextureManager.h"
#include "Rendering/Quad.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		SpriteRenderer( const std::string& filePath );

		void Render( const Renderer& renderer, const TextureManager& textureManager, const Camera& camera ) const;
		void Update( double dt );

	private:
	private:
		std::string m_FilePath = RESOURCE_DIR "textures/base.png";

		Quad m_Quad;
	};
}
