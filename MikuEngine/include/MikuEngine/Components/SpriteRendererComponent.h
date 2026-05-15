#pragma once

#include <optional>

#include "Core.h"
#include "UUID.h"

#include "Components/BaseComponent.h"
#include "glm/ext/vector_float4.hpp"

namespace MikuEngine
{
	class MIKU_API SpriteRendererComponent : public BaseComponent
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent( const SpriteRendererComponent& ) = default;

		SpriteRendererComponent( const glm::vec4& tint ) : Tint( tint ) {};
		SpriteRendererComponent( UUID uuid ) : TextureIdentifier( uuid ) {};
		SpriteRendererComponent( const glm::vec4& tint, const UUID& textureIdentifier ) : Tint( tint ), TextureIdentifier( textureIdentifier ) {};

		std::optional<UUID> TextureIdentifier;
		std::optional<UUID> MaterialUUID;
		glm::vec4 Tint{ 1.0f };
	};
}
