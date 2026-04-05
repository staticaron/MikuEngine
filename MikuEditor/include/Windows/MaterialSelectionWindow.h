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
	class MIKU_API MaterialSelectionWindow
	{
	public:
		MaterialSelectionWindow( MikuEngine::UUID entityUUID, std::function<void( MikuEngine::Scene& scene, MikuEngine::UUID itemUUID )> onItemSelected );
		WindowResponse RenderMaterialSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;
		std::function<void( MikuEngine::Scene& scene, MikuEngine::UUID itemUUID )> onItemSelected;

		bool m_IsOpen = true;
	};
}
