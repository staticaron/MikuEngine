#include "Entity.h"

#include "Components.h"

namespace MikuEngine
{
	Entity::Entity( entt::entity entity, Scene* parentScene ) : m_Entity( entity ), m_ParentScene( parentScene ) {}

	std::string Entity::GetNamedIdentifier() const
	{
		auto dc = GetReadOnlyComponent<DataComponent>();
		auto ic = GetReadOnlyComponent<IDComponent>();

		return dc.EntityName + "##" + std::to_string( ic.ID );
	}
	Entity::Entity( UUID uuid ) {}
}
