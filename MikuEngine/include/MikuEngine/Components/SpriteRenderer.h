#pragma once

#include <string>

#include "GameObjects/Camera.h"
#include "glm/glm.hpp"
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

	private:
		std::string m_FilePath = "";

		Quad m_Quad;
	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent( const SpriteRendererComponent& ) = default;

		SpriteRendererComponent( const glm::vec4& tint ) : Tint( tint ) {};
		SpriteRendererComponent( const std::string& textureIdentifier ) : TextureIdentifier( textureIdentifier ) {};
		SpriteRendererComponent( const glm::vec4& tint, const std::string& textureIdentifier ) : Tint( tint ), TextureIdentifier( textureIdentifier ) {};

		glm::vec4 Tint{ 1.0f };
		std::string TextureIdentifier = "miku";
	};
}
