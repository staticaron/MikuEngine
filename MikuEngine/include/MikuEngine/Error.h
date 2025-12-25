#include <iostream>

#include "glad/glad.h"
#include "Logger.h"

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API Error
	{
	public:
		static void LogOpenGLError( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam )
		{
			if ( id == 131169 || id == 131185 || id == 131218 || id == 131204 ) return;

			MIKU_ERROR( "---------------" );
			MIKU_ERROR( "Debug message ({}) : {}", id, message );

			switch ( source )
			{
			case GL_DEBUG_SOURCE_API:
				MIKU_ERROR( "Source: API" );
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				MIKU_ERROR( "Source: Window System" );
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				MIKU_ERROR( "Source: Shader Compiler" );
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				MIKU_ERROR( "Source: Third Party" );
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				MIKU_ERROR( "Source: Application" );
				break;
			case GL_DEBUG_SOURCE_OTHER:
				MIKU_ERROR( "Source: Other" );
				break;
			}

			switch ( type )
			{
			case GL_DEBUG_TYPE_ERROR:
				MIKU_ERROR( "Type: Error" );
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				MIKU_ERROR( "Type: Deprecated Behaviour" );
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				MIKU_ERROR( "Type: Undefined Behaviour" );
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				MIKU_ERROR( "Type: Portability" );
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				MIKU_ERROR( "Type: Performance" );
				break;
			case GL_DEBUG_TYPE_MARKER:
				MIKU_ERROR( "Type: Marker" );
				break;
			case GL_DEBUG_TYPE_PUSH_GROUP:
				MIKU_ERROR( "Type: Push Group" );
				break;
			case GL_DEBUG_TYPE_POP_GROUP:
				MIKU_ERROR( "Type: Pop Group" );
				break;
			case GL_DEBUG_TYPE_OTHER:
				MIKU_ERROR( "Type: Other" );
				break;
			}

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:
				MIKU_ERROR( "Severity: high" );
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				MIKU_ERROR( "Severity: medium" );
				break;
			case GL_DEBUG_SEVERITY_LOW:
				MIKU_ERROR( "Severity: low" );
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				MIKU_ERROR( "Severity: notification" );
				break;
			}

			std::cout << std::endl;
		}

		static void LogGLFWErorr( int code, const char* message ) { MIKU_ERROR( "GLFW ERROR, \nCode {} \nMessage {}", code, message ); }
	};
}
