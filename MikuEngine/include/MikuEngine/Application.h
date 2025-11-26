#pragma once

#include <chrono>
#include <memory>
#include <type_traits>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "AppLevelStuff.h"
#include "Core.h"
#include "DataContainer.h"
#include "Layer.h"
#include "Rendering/FrameBuffer.h"

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
			m_Layers.push_back( std::make_unique<TLayer>() );
		}

		std::unique_ptr<Layer>& GetLayer( unsigned int index ) { return m_Layers[ index ]; }

		AppLevelStuff& GetAppLevelStuff() { return m_AppLevelStuff; }

	private:
		void RenderTemp();
		void CalculateDT();
		void Update();
		void Render();
		void RenderGeometry();
		void RenderImGui();

	private:
		GLFWwindow* m_Window = nullptr;

		// Containers
		AppLevelStuff m_AppLevelStuff;
		DataContainer m_DataContainer;

		// Rendering
		FrameBuffer m_FrameBuffer;

		// Delta Time
		double m_DeltaTime = 0.0;
		std::chrono::high_resolution_clock::time_point LAST;
		std::chrono::high_resolution_clock::time_point NOW;

		// Layers
		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	Application* EntryPoint();
}
