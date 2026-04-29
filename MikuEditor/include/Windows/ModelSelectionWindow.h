#pragma once

#include "Core.h"
#include "MikuEngine/UUID.h"

#include "WindowResponse.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
}

namespace MikuEditor
{
	class MIKU_API ModelSelectionWindow
	{
	public:
		ModelSelectionWindow( std::function<void( MikuEngine::UUID )> onModelSelection );
		WindowResponse RenderModelSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		std::function<void( MikuEngine::UUID )> m_OnModelSelection;

		bool m_IsOpen = true;
	};
}
