#pragma once

#include <alloca.h>
#include <string>
#include <string_view>

namespace MikuEngine
{
	class Shader
	{
	public:
		void ParseShader( std::string_view filepath, std::string& vs, std::string& gs, std::string& fs );

		unsigned int CompileShader( const std::string& source, unsigned int type );

		unsigned int CreateShader( const std::string& vs, const std::string& gs, const std::string& fs );

		void LoadFromFile( std::string_view filepath );

		void Bind() const;
		void UnBind() const;

	private:
		unsigned int m_RendererID = 0;
	};
}
