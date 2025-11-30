#pragma once

#include <string>

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API DataComponent
	{
		DataComponent() = default;
		DataComponent( const DataComponent& ) = default;

		DataComponent( const std::string& entitiyName ) : EntityName( entitiyName ) {}

		std::string EntityName;
		unsigned int UUID;
	};
}
