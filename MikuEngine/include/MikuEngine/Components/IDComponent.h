#pragma once

#include "Components/BaseComponent.h"
#include "UUID.h"

namespace MikuEngine
{
	struct IDComponent : BaseComponent
	{
		IDComponent() = default;
		IDComponent( const IDComponent& ) = default;

		IDComponent( UUID uuid ) : ID( uuid ) {}

		UUID ID;
	};
}
