#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace MikuEngine
{
	class Application
	{
	public:
		Application();
		~Application();

		void Init();
		void Destroy();

		void Update( double dt );
		void Render();
		void RenderImGui();

	private:
		GLFWwindow* m_Window = nullptr;
	};

	Application* EntryPoint();
}
