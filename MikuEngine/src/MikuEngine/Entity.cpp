#include "Entity.h"

#include "Components.h"

namespace MikuEngine
{
	Entity::Entity( UUID uuid, entt::entity entity, Scene* parentScene, const std::string& name, std::optional<UUID> parentUUID ) : m_Entity( entity ), m_ParentScene( parentScene )
	{
		SetUUID( uuid );

		auto& dataC = GetOrAddComponent<DataComponent>();
		dataC.EntityName = name;
		dataC.ParentUUID = parentUUID;
	}

	std::string Entity::GetNamedIdentifier() const
	{
		auto dc = GetReadOnlyComponent<DataComponent>();
		auto ic = GetReadOnlyComponent<IDComponent>();

		return dc.EntityName + "##" + std::to_string( ic.ID );
	}
}
