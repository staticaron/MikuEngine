#pragma once

#include <chrono>
#include <memory>
#include <type_traits>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Layer.h"
#include "Managers/ImguiManager.h"
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
		void CalculateDT();
		void Update();
		void Render();
		void RenderGeometry();
		void RenderImGui();

	private:
		GLFWwindow* m_Window = nullptr;
		ImguiManager m_ImGuiManager;

		double m_DeltaTime = 0.0;
		std::chrono::high_resolution_clock::time_point LAST;
		std::chrono::high_resolution_clock::time_point NOW;

		Renderer m_Renderer;

		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	Application* EntryPoint();
}
