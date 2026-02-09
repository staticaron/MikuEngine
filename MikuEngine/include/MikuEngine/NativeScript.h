#pragma once

#include "Core.h"

#include "Entity.h"

namespace MikuEngine
{
	class MIKU_API NativeScript
	{
	public:
		virtual void OnReady() {};
		virtual void OnUpdate( double dt ) {};
		virtual void OnDestroy() {};

	protected:
		std::optional<Entity> m_Entity;

		friend class NativeScriptComponent;
	};
}
