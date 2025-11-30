#pragma once

#include "entt/entt.hpp"

#include "Core.h"

#include "AppLevelStuff.h"
#include "Helpers/SceneSerializer.h"

namespace MikuEngine
{
	class MIKU_API Scene
	{
	public:
		Scene();
		~Scene() = default;

		virtual void Update( double dt ) = 0;
		virtual void Render( AppLevelStuff& appLevelStuff ) const = 0;
		virtual void RenderImGui( const AppLevelStuff& appLevelStuff ) = 0;

	private:
	protected:
		entt::registry m_Registry;
		SceneSerializer m_SceneSerializer;

		friend class SceneSerializer;
	};
}
