#pragma once

#include "Core.h"

#include "Scene/Scene.h"

namespace MikuEngine
{
	class AppLevelStuff;
}

namespace MikuEngine
{
	class MIKU_API Layer
	{
	public:
		Layer( Scene* scene ) : m_Scene( scene ) {}
		virtual ~Layer() {};

		virtual void Update( double dt ) = 0;
		virtual void Render( AppLevelStuff& appLevelStuff ) const = 0;
		virtual void RenderImgui( const AppLevelStuff& appLevelStuff ) = 0;

	protected:
		Scene* m_Scene;
	};
}
