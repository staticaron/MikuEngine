#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Core.h"

#include "Components/BaseComponent.h"

namespace MikuEngine
{
	class MIKU_API SpriteRendererComponent : public BaseComponent
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent( const SpriteRendererComponent& ) = default;

		SpriteRendererComponent( const glm::vec4& tint ) : Tint( tint ) {};
		SpriteRendererComponent( const std::string& textureIdentifier ) : TextureIdentifier( textureIdentifier ) {};
		SpriteRendererComponent( const glm::vec4& tint, const std::string& textureIdentifier ) : Tint( tint ), TextureIdentifier( textureIdentifier ) {};

		glm::vec4 Tint{ 1.0f };
		std::string TextureIdentifier = "";
	};
}
