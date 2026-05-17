#pragma once

#include <optional>
#include <string>

#include "Core.h"

#include "BaseComponent.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API DataComponent : public BaseComponent
	{
		DataComponent() = default;
		DataComponent( const DataComponent& ) = default;

		DataComponent( const std::string& entitiyName ) : EntityName( entitiyName ) {}
		DataComponent( const std::string& entitiyName, std::optional<UUID> parentUUID ) : EntityName( entitiyName ), ParentUUID( parentUUID ) {}

		std::string EntityName = "NewObject";
		std::optional<UUID> ParentUUID = std::nullopt;
	};
}
