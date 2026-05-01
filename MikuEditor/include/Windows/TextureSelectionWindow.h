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
		TextureSelectionWindow( std::function<void( MikuEngine::UUID selectedTextureItem )> onTextureSelection );

		WindowResponse RenderTextureSelectionWindow( const std::string& identifier, const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		std::string m_WindowIdentifier;
		std::function<void( MikuEngine::UUID selectedTextureItem )> m_OnTextureSelection;

		bool m_IsOpen = true;

		static constexpr unsigned int m_ImageButtonSpacing = 4;
		static constexpr unsigned int m_ImageSize = 100;
	};
}
