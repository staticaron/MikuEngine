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
		MaterialSelectionWindow( std::function<void( MikuEngine::UUID itemUUID )> onMaterialSelection );
		WindowResponse RenderMaterialSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		std::function<void( MikuEngine::UUID itemUUID )> onItemSelected;

		bool m_IsOpen = true;
	};
}
