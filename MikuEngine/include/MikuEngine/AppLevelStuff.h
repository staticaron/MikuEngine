#pragma once

#include "Managers/ImguiManager.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class AppLevelStuff
	{
	public:
		Renderer* GetRenderer() { return m_Renderer; }
		TextureManager* GetTextureManager() { return m_TextureManager; }
		ImguiManager* GetImGuiManager() { return m_ImGuiManager; }

		void SetRenderer( Renderer* renderer ) { m_Renderer = renderer; }
		void SetTextureManager( TextureManager* textureManager ) { m_TextureManager = textureManager; }
		void SetImguiManager( ImguiManager* imguiManager ) { m_ImGuiManager = imguiManager; }

	private:
		Renderer* m_Renderer = nullptr;
		TextureManager* m_TextureManager = nullptr;
		ImguiManager* m_ImGuiManager = nullptr;
	};
}
