#include "Application.h"

#include <chrono>

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "Logger.h"

#include "Error.h"
#include "Layer.h"
#include "Rendering/Primitives/Vertex.h"
#include "Rendering/Shader.h"
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

	AppLevelStuff& Application::GetAppLevelStuff()
	{
		return s_Application->m_AppLevelStuff;
	}

	void Application::RenderTemp()
	{
		std::array<Vertex, 4> Verts = Quad::GetVerts();
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
		shader.LoadFromFile( RESOURCE_DIR "/shaders/base.shader" );

		vb.PutData( Verts.data(), Verts.size() * sizeof( Vertex ) );
		ib.PutData( Indices.data(), Indices.size() );

		glm::vec3 position{};
		glm::vec3 rotation{};
		glm::vec3 scale{ 300.0f };

		while ( !glfwWindowShouldClose( m_Window ) )
		{
			m_SceneFBO.Bind();

			glClearColor( 0, 1, 1, 1 );
			glClear( GL_COLOR_BUFFER_BIT );

			glm::mat4 proj = glm::ortho( 0.0f, GetDataContainer().GetViewportSize().x, GetDataContainer().GetViewportSize().y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 view = glm::mat4( 1.0f );

			glm::mat4 transformMat = glm::translate( glm::mat4( 1.0f ), position );
			glm::mat4 rotationMat = glm::rotate( glm::mat4( 1.0f ), 0.0f, glm::vec3( 1.0f ) );
			glm::mat4 scaleMat = glm::scale( glm::mat4( 1.0f ), scale );
			glm::mat4 model = transformMat * rotationMat * scaleMat;

			glm::mat4 mvp = proj * view * model;

			shader.SetUniform<glm::mat4>( "u_MVP", mvp );

			auto texture = m_AppLevelStuff.GetAssetPoolManager().GetTextureManager().GetTextureByName( "miku" );
			texture.value()->texture.Bind( 0 );

			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			m_AppLevelStuff.GetRenderer().Draw( va, ib, shader );

			m_SceneFBO.UnBind();

			glClearColor( 0.0f, 0.5f, 0.5f, 1 );
			glClear( GL_COLOR_BUFFER_BIT );

			m_AppLevelStuff.GetImGuiManager().PrepareFrame();

			ImGui::Begin( "Settings" );

			ImGui::DragFloat3( "Position", &position[ 0 ] );
			ImGui::DragFloat3( "Rotation", &rotation[ 0 ] );
			ImGui::DragFloat3( "Scale", &scale[ 0 ] );

			ImGui::End();

			m_AppLevelStuff.GetImGuiManager().RenderFrameBuffer( m_SceneFBO );
			m_AppLevelStuff.GetImGuiManager().RenderFrame();

			glfwPollEvents();
			glfwSwapBuffers( m_Window );
		}
	}

	void Application::Init()
	{
		if ( !glfwInit() ) return;

		glfwSetErrorCallback( Error::LogGLFWErorr );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		glfwWindowHint( GLFW_DECORATED, GLFW_TRUE );
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

		int major = 0, minor = 0, rev = 0;
		glfwGetVersion( &major, &minor, &rev );
		MIKU_CORE_INFO( "GLFW Loaded! #{}{}{}", major, minor, rev );

		const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );

		glfwWindowHint( GLFW_RED_BITS, mode->redBits );
		glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
		glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
		glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );
		glfwWindowHint( GLFW_MAXIMIZED, 1 );

		m_Window = glfwCreateWindow( 800, 600, "Miku Engine", NULL, NULL );

		if ( !m_Window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( m_Window );
		glfwSwapInterval( 0 );

		// Load OpenGL
		if ( !gladLoadGL() )
			return;
		else
			MIKU_CORE_INFO( "GLAD Loaded!" );

		// Enable Debugging
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( Error::LogOpenGLError, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );

		glClipControl( GL_UPPER_LEFT, GL_ZERO_TO_ONE );

		// Enable Alpha Blending
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		// Enable Depth Testing
		glEnable( GL_DEPTH_TEST );

		glEnable( GL_CULL_FACE );

		// Initializing of member vars
		LAST = NOW = m_LastRecordTimePoint = std::chrono::high_resolution_clock::now();

		m_SceneFBO.Init();
		m_GameFBO.Init();

		m_AppLevelStuff.GetAssetPoolManager().Init();
		m_AppLevelStuff.GetRenderer().Init();
		m_AppLevelStuff.GetImGuiManager().Init( m_Window );

		// RenderTemp();
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
		m_AppLevelStuff.DeltaTime = static_cast<double>( m_DeltaTime );

		// CALCULATE FPS PER SECOND

		constexpr float FPS_TEST_TIME_GAP = 0.5f;

		m_FrameCountPerSecond++;
		auto timeGap = std::chrono::duration<double>( NOW - m_LastRecordTimePoint ).count();

		if ( timeGap > FPS_TEST_TIME_GAP )
		{
			m_AppLevelStuff.FPS = m_FrameCountPerSecond / FPS_TEST_TIME_GAP;
			m_FrameCountPerSecond = 0;
			m_LastRecordTimePoint = NOW;
		}
	}

	void Application::Update()
	{
		m_AppLevelStuff.GetCentralInput().Update( m_DeltaTime );

		m_AppLevelStuff.GetAssetPoolManager().GetShaderManager().InitFrame();
		m_AppLevelStuff.GetAssetPoolManager().GetMaterialManager().InitFrame();

		for ( int x = 0; x < m_Layers.size(); x++ )
			m_Layers[ x ]->Update( m_DeltaTime );
	}

	void Application::Render()
	{
		RenderGeometry();
		RenderImGui();

		glfwSwapBuffers( m_Window );
	}

	void Application::RenderGeometry()
	{
		for ( int x = 0; x < m_Layers.size(); x++ )
			m_Layers[ x ]->Render( m_AppLevelStuff );
	}

	void Application::RenderImGui()
	{
		glClearColor( 0.3f, 0.3f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		m_AppLevelStuff.GetImGuiManager().PrepareFrame();

		// Render ImGui for each layer
		for ( int x = 0; x < m_Layers.size(); x++ )
			m_Layers[ x ]->RenderImgui( m_AppLevelStuff );

		m_AppLevelStuff.GetImGuiManager().RenderFrame();
	}
}
