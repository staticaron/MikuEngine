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
		ShaderSelectionWindow( MikuEngine::UUID entityUUID );
		WindowResponse RenderShaderSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;

		bool m_IsOpen = true;
	};
}
