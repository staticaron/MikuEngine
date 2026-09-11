#pragma once

#include <alloca.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"

#include "Core.h"
#include "IAsset.h"
#include "UUID.h"

#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	class ShaderManager;
}

namespace MikuEngine
{
	struct MIKU_API ShaderUniform
	{
		std::string Name;
		unsigned int Index;
		unsigned int Type;
	};

	class MIKU_API Shader : public IAsset
	{
	public:
		Shader() : IAsset( AssetType::SHADER ) {};
		Shader( UUID uuid, const std::filesystem::path& path );

		void Load( const std::filesystem::path& filepath );
		void Destroy();

		void PrepareUniforms();

		void Bind() const;
		void UnBind() const;

		const UUID& GetUUID() const { return m_ShaderUUID; }

		std::string GetName() const override;
		const std::filesystem::path& GetPath() const override;

		unsigned int GetUniformLocation( const std::string& uniformName )
		{
			auto existing = m_Uniforms.find( uniformName );
			if ( existing != m_Uniforms.end() )
				return existing->second.Index;

			Bind();
			int uniformLocation = glGetUniformLocation( m_RendererID, uniformName.c_str() );
			return uniformLocation;
		}

		const std::unordered_map<std::string, ShaderUniform>& GetUniforms() const { return m_Uniforms; }

		template <typename T>
		void SetUniform( const std::string& uniformName, T value )
		{
			static_assert( sizeof( T ) == 0, "No specialization of this type!" );
		}

	private:
		void ParseShader( std::string_view filepath, std::string& vs, std::string& gs, std::string& fs );

		unsigned int CompileShader( const std::string& source, unsigned int type );
		unsigned int CreateShader( const std::string& vs, const std::string& gs, const std::string& fs );

		void SetPath( const std::filesystem::path& path ) { m_FilePath = path; }

	private:
		UUID m_ShaderUUID{};
		std::filesystem::path m_FilePath{};

		unsigned int m_RendererID = 0;

		std::unordered_map<std::string, ShaderUniform> m_Uniforms = {};

		friend ShaderManager;
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
	inline void Shader::SetUniform<glm::vec2>( const std::string& uniformName, glm::vec2 value )
	{
		Bind();
		auto uniformLocation = GetUniformLocation( uniformName );
		glUniform2f( uniformLocation, value.x, value.y );
	}

	template <>
	inline void Shader::SetUniform<glm::mat4>( const std::string& uniformName, glm::mat4 value )
	{
		Bind();
		auto uniformLocation = GetUniformLocation( uniformName );
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &value[ 0 ][ 0 ] );
	}
}
