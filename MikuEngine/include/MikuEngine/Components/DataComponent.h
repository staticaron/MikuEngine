#pragma once

#include <string>

#include "Core.h"

#include "Components.h"

namespace MikuEngine
{
	struct MIKU_API DataComponent : BaseComponent
	{
		DataComponent() = default;
		DataComponent( const DataComponent& ) = default;

		DataComponent( const std::string& entitiyName ) : EntityName( entitiyName ) {}

		std::string EntityName;
	};
}
