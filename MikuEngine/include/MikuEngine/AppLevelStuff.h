#pragma once

#include "Core.h"
#include "Input/CentralInput.h"
#include "Managers/AssetPoolManager.h"
#include "Managers/ImguiManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class MIKU_API AppLevelStuff
	{
	public:
		Renderer& GetRenderer() { return m_Renderer; }
		const Renderer& GetRenderer() const { return m_Renderer; }

		ImguiManager& GetImGuiManager() { return m_ImGuiManager; }
		const ImguiManager& GetImGuiManager() const { return m_ImGuiManager; }

		AssetPoolManager& GetAssetPoolManager() { return m_AssetPoolManager; }
		const AssetPoolManager& GetAssetPoolManager() const { return m_AssetPoolManager; }

		CentralInput& GetCentralInput() { return m_CentralInput; }
		const CentralInput& GetCentralInput() const { return m_CentralInput; }

		unsigned int GetFPS() const { return FPS; }
		double GetDeltaTime() const { return DeltaTime; }

	private:
		Renderer m_Renderer;
		ImguiManager m_ImGuiManager;
		AssetPoolManager m_AssetPoolManager;
		CentralInput m_CentralInput;

		unsigned int FPS;
		double DeltaTime;

		friend class Application;
	};
}
