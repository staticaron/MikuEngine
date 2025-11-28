#pragma once

#include "Core.h"

#include "AppLevelStuff.h"

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
	private:
	};
}
