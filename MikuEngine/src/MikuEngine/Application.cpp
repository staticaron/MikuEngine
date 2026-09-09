#include "Application.h"

#include <chrono>

#include "GLFW/glfw3.h"

#include "EngineConfigLoader.h"
#include "Error.h"
#include "Layer.h"
#include "Logger.h"

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

	void Application::Init()
	{
		auto engineConfig = EngineConfigLoader::LoadConfig();
		m_DataContainer.SetEngineConfig( engineConfig );

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
		MIKU_CORE_DEBUG( "GLFW Loaded! #{}{}{}", major, minor, rev );

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
			MIKU_CORE_DEBUG( "GLAD Loaded!" );

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

		// Enable Face culling of back faces
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );

		// Initializing of member vars
		LAST = NOW = m_LastRecordTimePoint = std::chrono::high_resolution_clock::now();

		m_SceneFBO.Init();
		m_GameFBO.Init();

		m_AppLevelStuff.GetAssetPoolManager().Init();
		m_AppLevelStuff.GetRenderer().Init();
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
		m_AppLevelStuff.GetAssetPoolManager().InitFrame();

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
