#pragma once

#include "Core.h"
#include "Managers/ImguiManager.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class MIKU_API AppLevelStuff
	{
	public:
		Renderer& GetRenderer() { return m_Renderer; }
		const Renderer& GetRenderer() const { return m_Renderer; }

		TextureManager& GetTextureManager() { return m_TextureManager; }
		const TextureManager& GetTextureManager() const { return m_TextureManager; }

		ImguiManager& GetImGuiManager() { return m_ImGuiManager; }
		const ImguiManager& GetImGuiManager() const { return m_ImGuiManager; }

		void SetRenderer( Renderer& renderer ) { m_Renderer = renderer; }
		void SetTextureManager( TextureManager& textureManager ) { m_TextureManager = textureManager; }
		void SetImguiManager( ImguiManager& imguiManager ) { m_ImGuiManager = imguiManager; }

		unsigned int GetFPS() const { return FPS; }

	private:
		Renderer m_Renderer;
		TextureManager m_TextureManager;
		ImguiManager m_ImGuiManager;

		unsigned int FPS;

		friend class Application;
	};
}
