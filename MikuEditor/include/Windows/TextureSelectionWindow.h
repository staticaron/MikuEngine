#pragma once

#include <functional>

#include "MikuEngine/Core.h"
#include "MikuEngine/UUID.h"

namespace MikuEngine
{
	class Scene;
	class AppLevelStuff;
}

namespace MikuEditor
{
	enum class TextureSelectionWindowResponse
	{
		INPROGRESS = 0,
		COMPLETED,
		CLOSED,
		ERROR
	};

	class MIKU_API TextureSelectionWindow
	{
	public:
		TextureSelectionWindow( MikuEngine::UUID entityUUID );

		TextureSelectionWindowResponse RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;

		bool m_IsOpen = true;

		static constexpr unsigned int m_ImageButtonSpacing = 4;
		static constexpr unsigned int m_ImageSize = 100;
	};
}
