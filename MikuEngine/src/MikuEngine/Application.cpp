#include "Application.h"

#include <chrono>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "spdlog/spdlog.h"

#include "Error.h"
#include "Rendering/Shader.h"
#include "Rendering/Vertex.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::RenderTemp()
	{
		std::array<Vertex, 4> Verts;

		// clang-format off
		Verts[ 0 ] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } };
		Verts[ 1 ] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } };
		Verts[ 2 ] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f } };
		Verts[ 3 ] = { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f } };
		// clang-format on

		std::array<unsigned int, 6> Indices = { 0, 1, 2, 2, 3, 0 };

		VertexBuffer vb( sizeof( Vertex ) * 4, nullptr );
		IndexBuffer ib( 6, nullptr );
		VertexBufferLayout vbl;
		VertexArray va;

		vbl.Add<float>( 3 );
		vbl.Add<float>( 2 );

		va.Setup( vb, vbl );

		Shader shader;
		shader.LoadFromFile( RESOURCE_DIR "shaders/base.shader" );

		vb.PutData( Verts.data(), Verts.size() * sizeof( Vertex ) );
		ib.PutData( Indices.data(), Indices.size() );

		while ( true )
		{
			glClear( GL_COLOR_BUFFER_BIT );

			m_Renderer.Draw( va, ib, shader );

			glfwSwapBuffers( m_Window );
		}
	}

	void Application::Init()
	{
		if ( !glfwInit() )
			return;

		glfwSetErrorCallback( Error::LogGLFWErorr );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

		int major = 0, minor = 0, rev = 0;
		glfwGetVersion( &major, &minor, &rev );
		spdlog::info( "GLFW Loaded! #{}{}{}", major, minor, rev );

		m_Window = glfwCreateWindow( 640, 480, "MikuEngine", NULL, NULL );

		if ( !m_Window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( m_Window );

		if ( !gladLoadGL() )
			return;
		else
			spdlog::info( "GLAD Loaded!" );

		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( Error::LogOpenGLError, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );

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
		m_DeltaTime = std::clamp( m_DeltaTime, 0.0, 0.01 );
	}

	void Application::Update()
	{
		for ( int x = 0; x < m_Layers.size(); x++ )
		{
			m_Layers[ x ]->Update( m_DeltaTime );
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

		for ( int x = 0; x < m_Layers.size(); x++ )
		{
			m_Layers[ x ]->Render( m_Renderer );
		}
	}

	void Application::RenderImGui()
	{
		m_ImGuiManager.PrepareFrame();

		// Render Imgui Here...
		for ( int x = 0; x < m_Layers.size(); x++ )
		{
			m_Layers[ x ]->RenderImgui();
		}

		ImGui::ShowDemoWindow();

		m_ImGuiManager.RenderFrame();
	}
}
