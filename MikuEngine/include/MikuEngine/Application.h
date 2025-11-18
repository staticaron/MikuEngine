#pragma once

#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "LayerStack.h"
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

		LayerStack& GetLayerStack()
		{
			return m_LayerStack;
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

		LayerStack m_LayerStack;
	};

	Application* EntryPoint();
}
