#pragma once

#include <alloca.h>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Core.h"
#include "glm/glm.hpp"

#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	struct MIKU_API ShaderUniform
	{
		std::string Name;
		unsigned int Index;
		unsigned int Type;
	};

	class MIKU_API Shader
	{
	public:
		void ParseShader( std::string_view filepath, std::string& vs, std::string& gs, std::string& fs );

		unsigned int CompileShader( const std::string& source, unsigned int type );
		unsigned int CreateShader( const std::string& vs, const std::string& gs, const std::string& fs );

		void LoadFromFile( std::string_view filepath );

		void PrepareUniforms();

		void Bind() const;
		void UnBind() const;

		unsigned int GetUniformLocation( const std::string& uniformName )
		{
			auto existing = m_Uniforms.find( uniformName );
			if ( existing != m_Uniforms.end() ) return existing->second.Index;

			Bind();
			int uniformLocation = glGetUniformLocation( m_RendererID, uniformName.c_str() );
			return uniformLocation;
		}

		const std::unordered_map<std::string, ShaderUniform>& GetUniforms() const { return m_Uniforms; }

		template <typename T>
		void SetUniform( const std::string& uniformName, T value )
		{
			static_assert( false );
		}

	private:
		unsigned int m_RendererID = 0;
		std::unordered_map<std::string, ShaderUniform> m_Uniforms = {};
	};

	template <>
	inline void Shader::SetUniform<float>( const std::string& uniformName, float value )
	{
		Bind();
		auto uniformLocation = GetUniformLocation( uniformName );
		glUniform1f( uniformLocation, value );
	}

	template <>
	inline void Shader::SetUniform<unsigned int>( const std::string& uniformName, unsigned int value )
	{
		Bind();
		auto uniformLocation = GetUniformLocation( uniformName );
		glUniform1i( uniformLocation, value );
	}

	template <>
	inline void Shader::SetUniform<glm::mat4>( const std::string& uniformName, glm::mat4 value )
	{
		Bind();
		auto uniformLocation = GetUniformLocation( uniformName );
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &value[ 0 ][ 0 ] );
	}
}
