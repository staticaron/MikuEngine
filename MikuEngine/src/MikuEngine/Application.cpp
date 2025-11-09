#include "Application.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

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

		float positions[ 6 ] = { 0.5f, 0.5f, -0.5f, 0.5f, 0.0f, -0.5f };
		unsigned int indices[ 3 ] = { 0, 1, 2 };

		VertexBuffer vb( sizeof( float ) * 6, positions );
		VertexBufferLayout vbl;
		IndexBuffer ib( 3, indices );
		VertexArray va;
		Shader shader;

		shader.LoadFromFile( RESOURCE_DIR "shaders/base.shader" );
		Renderer renderer;

		while ( !glfwWindowShouldClose( m_Window ) )
		{
			glClear( GL_COLOR_BUFFER_BIT );

			vb.Bind();
			vbl.Add<float>( 2 );
			renderer.Draw( va, ib, shader );

			glfwSwapBuffers( m_Window );

			glfwPollEvents();
		}

		glfwTerminate();
	}

	Application::~Application()
	{
	}

	void Application::Update( double dt )
	{
	}

	void Application::Render()
	{
		glClearColor( 0.1f, 0.3f, 0.9f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( m_Window );

		glfwPollEvents();
	}

	void Application::RenderImGui()
	{
	}
}
