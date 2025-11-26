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
	Application* s_Application;

	Application::Application()
	{
		s_Application = this;
	}

	Application::~Application() {}

	Application* Application::GetApplication()
	{
		return s_Application;
	}

	DataContainer& Application::GetDataContainer()
	{
		return s_Application->m_DataContainer;
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

		VertexBuffer vb;
		vb.Init( sizeof( Vertex ) * 4, nullptr );

		IndexBuffer ib;
		ib.Init( 6, nullptr );

		VertexBufferLayout vbl;

		VertexArray va;
		va.Init();

		vbl.Add<float>( 3 );
		vbl.Add<float>( 2 );

		va.Setup( vb, vbl );

		Shader shader;
		shader.LoadFromFile( RESOURCE_DIR "shaders/base.shader" );

		vb.PutData( Verts.data(), Verts.size() * sizeof( Vertex ) );
		ib.PutData( Indices.data(), Indices.size() );

		while ( !glfwWindowShouldClose( m_Window ) )
		{
			glClearColor( 0, 1, 1, 1 );
			glClear( GL_COLOR_BUFFER_BIT );

			m_AppLevelStuff.GetRenderer().Draw( va, ib, shader );

			glfwPollEvents();
			glfwSwapBuffers( m_Window );
		}
	}

	void Application::Init()
	{
		// Load GLFW
		if ( !glfwInit() ) return;

		glfwSetErrorCallback( Error::LogGLFWErorr );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

		int major = 0, minor = 0, rev = 0;
		glfwGetVersion( &major, &minor, &rev );
		spdlog::info( "GLFW Loaded! #{}{}{}", major, minor, rev );

		// Create GLFW Window
		m_Window = glfwCreateWindow( 640, 480, "MikuEngine", NULL, NULL );

		if ( !m_Window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( m_Window );

		// Load OpenGL
		if ( !gladLoadGL() )
			return;
		else
			spdlog::info( "GLAD Loaded!" );

		// Enable Debugging
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( Error::LogOpenGLError, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );

		// Initializing of member vars
		LAST = NOW = std::chrono::high_resolution_clock::now();

		m_FrameBuffer.Init();

		m_AppLevelStuff.GetRenderer().Init();
		m_AppLevelStuff.GetTextureManager().LoadAllTextures();
		m_AppLevelStuff.GetImGuiManager().Init( m_Window );
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
			m_Layers[ x ]->Update( m_DeltaTime );
	}

	void Application::Render()
	{
		// Render Geometry on the new frame buffer
		m_FrameBuffer.Bind();
		RenderGeometry();
		m_FrameBuffer.UnBind();

		// Render IMGUI on default frame buffer
		RenderImGui();

		glfwSwapBuffers( m_Window );
	}

	void Application::RenderGeometry()
	{
		glClear( GL_COLOR_BUFFER_BIT );

		for ( int x = 0; x < m_Layers.size(); x++ )
			m_Layers[ x ]->Render( m_AppLevelStuff );
	}

	void Application::RenderImGui()
	{
		glClear( GL_COLOR_BUFFER_BIT );

		m_AppLevelStuff.GetImGuiManager().PrepareFrame();

		// Render Imgui Here...

		m_AppLevelStuff.GetImGuiManager().RenderFrameBuffer( m_FrameBuffer );

		for ( int x = 0; x < m_Layers.size(); x++ )
			m_Layers[ x ]->RenderImgui();

		ImGui::ShowDemoWindow();

		m_AppLevelStuff.GetImGuiManager().RenderFrame();
	}
}
