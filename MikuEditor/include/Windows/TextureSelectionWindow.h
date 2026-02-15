#pragma once

#include "MikuEngine/Core.h"
#include "MikuEngine/UUID.h"

#include "Windows/WindowResponse.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
}

namespace MikuEditor
{
	class MIKU_API TextureSelectionWindow
	{
	public:
		TextureSelectionWindow( MikuEngine::UUID entityUUID );

		WindowResponse RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;

		bool m_IsOpen = true;

		static constexpr unsigned int m_ImageButtonSpacing = 4;
		static constexpr unsigned int m_ImageSize = 100;
	};
}
