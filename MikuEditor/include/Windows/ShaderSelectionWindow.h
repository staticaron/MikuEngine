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
	class MIKU_API ShaderSelectionWindow
	{
	public:
		ShaderSelectionWindow( std::function<void( MikuEngine::UUID itemUUID )> onShaderSelection );
		WindowResponse RenderShaderSelectionWindow( const std::string& identifier, const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		std::function<void( MikuEngine::UUID shaderUUID )> onShaderSelection;

		bool m_IsOpen = true;
	};
}
