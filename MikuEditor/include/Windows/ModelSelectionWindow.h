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
		ModelSelectionWindow( MikuEngine::UUID entityUUID );
		WindowResponse RenderModelSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;

		bool m_IsOpen = true;
	};
}
