#pragma once

#include <functional>

#include "MikuEngine/Core.h"
#include "MikuEngine/UUID.h"

#include "MikuEngine/Scene/Scene.h"

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
		TextureSelectionWindow( MikuEngine::UUID entityUUID, std::function<void( MikuEngine::UUID )> textureSelectionCallback );

		TextureSelectionWindowResponse RenderTextureSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene );

	private:
		MikuEngine::UUID m_EntityUUID;
		std::function<void( MikuEngine::UUID )> m_TextureSetCallback;

		bool m_IsOpen = true;
	};
}
