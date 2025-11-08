#include "Application.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace MikuEngine
{
	Application::Application()
	{
		GLFWwindow* window;

		if ( !glfwInit() )
			return;

		window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );
		if ( !window )
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent( window );

		if ( !gladLoadGL( glfwGetProcAddress ) )
			return;

		while ( !glfwWindowShouldClose( window ) )
		{
			glClear( GL_COLOR_BUFFER_BIT );

			glfwSwapBuffers( window );

			glfwPollEvents();
		}

		glfwTerminate();
	}

	Application::~Application() {}
}
