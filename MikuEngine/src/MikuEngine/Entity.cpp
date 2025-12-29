#include "Entity.h"

#include "Components.h"
#include "Logger.h"

namespace MikuEngine
{
	Entity::Entity( UUID uuid, entt::entity entity, Scene* parentScene ) : m_Entity( entity ), m_ParentScene( parentScene )
	{
		SetUUID( uuid );
	}

	std::string Entity::GetNamedIdentifier() const
	{
		auto dc = GetReadOnlyComponent<DataComponent>();
		auto ic = GetReadOnlyComponent<IDComponent>();

		return dc.EntityName + "##" + std::to_string( ic.ID );
	}
}
