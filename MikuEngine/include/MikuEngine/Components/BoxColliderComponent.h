#pragma once

#include "glm/glm.hpp"

#include "Components/BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API BoxColliderComponent : public BaseComponent
	{
		BoxColliderComponent() = default;
		BoxColliderComponent( const BoxColliderComponent& ) = default;

		BoxColliderComponent( glm::vec2 offset, glm::vec2 bounds ) : m_Offset( offset ), m_Bounds( bounds ) {}

		glm::vec2 m_Offset;
		glm::vec2 m_Bounds;

		bool m_IsTriggerCollider;
	};
}
