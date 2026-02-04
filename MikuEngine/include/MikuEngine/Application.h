#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "AppLevelStuff.h"
#include "Core.h"
#include "DataContainer.h"
#include "Rendering/GameFBO.h"
#include "Rendering/SceneFBO.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class Layer;
}

namespace MikuEngine
{
	class MIKU_API Application
	{
	public:
		Application();
		~Application();

		void Init();
		void Run();

		static Application* GetApplication();
		static DataContainer& GetDataContainer();

		template <typename TLayer>
			requires( std::is_base_of_v<Layer, TLayer> )
		void PushLayer()
		{
			m_Layers.push_back( std::make_unique<TLayer>( &m_Scene ) );
		}

		std::unique_ptr<Layer>& GetLayer( unsigned int index ) { return m_Layers[ index ]; }

		GLFWwindow* GetWindow() { return m_Window; }
		AppLevelStuff& GetAppLevelStuff() { return m_AppLevelStuff; }

		SceneFBO& GetSceneFBO() { return m_SceneFBO; }
		GameFBO& GetGameFBO() { return m_GameFBO; }

	private:
		void RenderTemp();
		void CalculateDT();
		void Update();
		void Render();
		void RenderGeometry();
		void RenderImGui();

	private:
		GLFWwindow* m_Window = nullptr;

		AppLevelStuff m_AppLevelStuff;
		DataContainer m_DataContainer;

		SceneFBO m_SceneFBO;
		GameFBO m_GameFBO;

		Scene m_Scene;

		// Delta Time
		double m_DeltaTime = 0.0;
		std::chrono::high_resolution_clock::time_point LAST;
		std::chrono::high_resolution_clock::time_point NOW;

		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	Application* EntryPoint();
}
