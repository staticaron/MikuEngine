#pragma once

#include "Core.h"
#include "Entity.h"

namespace MikuEngine
{
	class MIKU_API ScriptableEntity
	{
	public:
		ScriptableEntity() {}

		virtual void OnCreate( Entity entity ) {};
		virtual void OnReady( Entity entity ) {};
		virtual void OnUpdate( Entity entity ) {};
		virtual void OnDestroy( Entity entity ) {};

	private:
	};
}
