#include "Application.h"

namespace MikuEngine
{
	Application::Application()
	{
		if ( !glfwInit() )
			return;

		m_Window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );

		if ( !m_Window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( m_Window );

		if ( !gladLoadGL() )
			return;

		while ( !glfwWindowShouldClose( m_Window ) )
		{
			Update( 0.0 );
			Render();
			RenderImGui();
		}

		glfwTerminate();
	}

	Application::~Application() {}

	void Application::Update( double dt ) {}

	void Application::Render()
	{
		glClearColor( 0.1f, 0.3f, 0.9f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( m_Window );

		glfwPollEvents();
	}

	void Application::RenderImGui() {}
}
