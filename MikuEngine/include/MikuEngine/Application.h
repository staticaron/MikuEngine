#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "AppLevelStuff.h"
#include "Core.h"
#include "DataContainer.h"
#include "Rendering/FrameBuffers/GameFBO.h"
#include "Rendering/FrameBuffers/SceneFBO.h"
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
		static AppLevelStuff& GetAppLevelStuff();

		template <typename TLayer>
			requires( std::is_base_of_v<Layer, TLayer> )
		void PushLayer()
		{
			m_Layers.push_back( std::make_unique<TLayer>() );
		}

		std::unique_ptr<Layer>& GetLayer( unsigned int index ) { return m_Layers[ index ]; }

		GLFWwindow* GetWindow() { return m_Window; }

		SceneFBO& GetSceneFBO() { return m_SceneFBO; }
		GameFBO& GetGameFBO() { return m_GameFBO; }

	private:
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

		double m_DeltaTime = 0.0;
		std::chrono::high_resolution_clock::time_point LAST;
		std::chrono::high_resolution_clock::time_point NOW;

		unsigned int m_FrameCountPerSecond = 0;
		std::chrono::high_resolution_clock::time_point m_LastRecordTimePoint;

		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	Application* EntryPoint();
}
