#pragma once

#include <chrono>
#include <memory>
#include <type_traits>
#include <vector>

#include "DataContainer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Rendering/FrameBuffer.h"

#include "Layer.h"
#include "Managers/ImguiManager.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class Application
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

		std::unique_ptr<Layer>& GetLayer( unsigned int index )
		{
			return m_Layers[ index ];
		}

	private:
		void RenderTemp();
		void CalculateDT();
		void Update();
		void Render();
		void RenderGeometry();
		void RenderImGui();

	private:
		GLFWwindow* m_Window = nullptr;

		ImguiManager m_ImGuiManager;

		DataContainer m_DataContainer;

		double m_DeltaTime = 0.0;
		std::chrono::high_resolution_clock::time_point LAST;
		std::chrono::high_resolution_clock::time_point NOW;

		TextureManager m_TextureManager;
		Renderer m_Renderer;
		FrameBuffer m_FrameBuffer;

		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	Application* EntryPoint();
}
