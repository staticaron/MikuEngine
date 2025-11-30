#include "Entity.h"

#include "Components/DataComponent.h"
#include "Components/TransformComponent.h"

namespace MikuEngine
{
	Entity::Entity( entt::registry& registry, const std::string& entitiyName )
	{
		m_Entity = registry.create();

		registry.emplace<DataComponent>( m_Entity, entitiyName );
		registry.emplace<TransformComponent>( m_Entity );
	}
}
