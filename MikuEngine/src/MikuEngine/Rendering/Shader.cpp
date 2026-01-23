#include "Rendering/Shader.h"
#include <fstream>
#include <sstream>

#include "Logger.h"

#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	void Shader::ParseShader( std::string_view filepath, std::string& vs, std::string& gs, std::string& fs )
	{
		enum class ShaderType
		{
			NONE = -1,
			VERTEX = 0,
			GEOMETRY = 1,
			FRAGMENT = 2,
		};

		std::ifstream stream( filepath.data() );

		if ( !stream.is_open() )
		{
			MIKU_CORE_ERROR( "Error loading the file at " );
			return;
		}

		ShaderType currentShader = ShaderType::NONE;
		std::stringstream ss[ 3 ];

		std::string line = "";

		while ( std::getline( stream, line ) )
		{
			if ( line.find( "#shader" ) != std::string::npos )
			{
				if ( line.find( "vertex" ) != std::string::npos )
					currentShader = ShaderType::VERTEX;
				else if ( line.find( "geometry" ) != std::string::npos )
					currentShader = ShaderType::GEOMETRY;
				else if ( line.find( "fragment" ) != std::string::npos )
					currentShader = ShaderType::FRAGMENT;
			}
			else
			{
				ss[ ( int )currentShader ] << line << "\n";
			}
		}

		vs = ss[ 0 ].str();
		gs = ss[ 1 ].str();
		fs = ss[ 2 ].str();
	}

	unsigned int Shader::CompileShader( const std::string& source, unsigned int type )
	{
		if ( source.empty() ) return 0;

		unsigned int shaderID = glCreateShader( type );
		const char* shaderSource = source.c_str();

		glShaderSource( shaderID, 1, &shaderSource, nullptr );
		glCompileShader( shaderID );

		int compileResult = true;
		glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compileResult );

		if ( compileResult == GL_FALSE )
		{
			int lengthOfMessage = 0;
			glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &lengthOfMessage );

			char* logMessage = ( char* )alloca( lengthOfMessage * sizeof( char ) );
			glGetShaderInfoLog( shaderID, lengthOfMessage, &lengthOfMessage, logMessage );

			MIKU_CORE_ERROR( "ERROR COMPILING SHADER {}", logMessage );
		}

		return shaderID;
	}

	unsigned int Shader::CreateShader( const std::string& vs, const std::string& gs, const std::string& fs )
	{
		unsigned int program = glCreateProgram();

		unsigned int vsID = 0;
		unsigned int gsID = 0;
		unsigned int fsID = 0;

		vsID = CompileShader( vs, GL_VERTEX_SHADER );
		if ( !gs.empty() ) gsID = CompileShader( gs, GL_GEOMETRY_SHADER );
		fsID = CompileShader( fs, GL_FRAGMENT_SHADER );

		glAttachShader( program, vsID );
		if ( !gs.empty() ) glAttachShader( program, gsID );
		glAttachShader( program, fsID );

		glLinkProgram( program );
		glValidateProgram( program );

		glDeleteShader( vsID );
		if ( !gs.empty() ) glDeleteShader( gsID );
		glDeleteShader( fsID );

		return program;
	}

	void Shader::LoadFromFile( std::string_view filepath )
	{
		std::string vs, gs, fs;
		ParseShader( filepath, vs, gs, fs );
		m_RendererID = CreateShader( vs, gs, fs );
	}

	void Shader::Bind() const
	{
		glUseProgram( m_RendererID );
	}

	void Shader::UnBind() const
	{
		glUseProgram( 0 );
	}
}
