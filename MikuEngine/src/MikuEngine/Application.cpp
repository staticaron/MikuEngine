#include "Application.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "LayerStack.h"
#include <chrono>

namespace MikuEngine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Init()
	{
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );

		if ( !glfwInit() )
			return;

		m_Window = glfwCreateWindow( 640, 480, "MikuEngine", NULL, NULL );

		if ( !m_Window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( m_Window );

		if ( !gladLoadGL() )
			return;

		LAST = NOW = std::chrono::high_resolution_clock::now();

		m_ImGuiManager.Init( m_Window );
	}

	void Application::Run()
	{
		while ( !glfwWindowShouldClose( m_Window ) )
		{
			CalculateDT();

			Update();

			Render();

			glfwPollEvents();
		}

		glfwTerminate();
	}

	void Application::CalculateDT()
	{
		LAST = NOW;
		NOW = std::chrono::high_resolution_clock::now();
		m_DeltaTime = std::chrono::duration<double>( NOW - LAST ).count();
	}

	void Application::Update()
	{
		for ( int x = 0; x < m_LayerStack.GetCount(); x++ )
		{
			m_LayerStack.GetLayer( x ).Update( m_DeltaTime );
		}
	}

	void Application::Render()
	{
		RenderGeometry();

		RenderImGui();

		glfwSwapBuffers( m_Window );
	}

	void Application::RenderGeometry()
	{
		glClear( GL_COLOR_BUFFER_BIT );

		for ( int x = 0; x < m_LayerStack.GetCount(); x++ )
		{
			m_LayerStack.GetLayer( x ).Render( m_Renderer );
		}
	}

	void Application::RenderImGui()
	{
		m_ImGuiManager.PrepareFrame();

		// Render Imgui Here...

		ImGui::ShowDemoWindow();

		m_ImGuiManager.RenderFrame();
	}
}
