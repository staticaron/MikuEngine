#pragma once

#include "Core.h"
#include "Managers/ImguiManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/ShaderManager.h"
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

		ShaderManager& GetShaderManager() { return m_ShaderManager; }
		const ShaderManager& GetShaderManager() const { return m_ShaderManager; }

		MaterialManager& GetMaterialManager() { return m_MaterialManager; }
		const MaterialManager& GetMaterialManager() const { return m_MaterialManager; }

		ImguiManager& GetImGuiManager() { return m_ImGuiManager; }
		const ImguiManager& GetImGuiManager() const { return m_ImGuiManager; }

		unsigned int GetFPS() const { return FPS; }
		double GetDeltaTime() const { return DeltaTime; }

	private:
		Renderer m_Renderer;
		TextureManager m_TextureManager;
		ShaderManager m_ShaderManager;
		ImguiManager m_ImGuiManager;
		MaterialManager m_MaterialManager;

		unsigned int FPS;
		double DeltaTime;

		friend class Application;
	};
}
