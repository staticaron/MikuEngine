#pragma once

#include <alloca.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

#include "glm/glm.hpp"

#include "Asset.h"
#include "Core.h"
#include "UUID.h"

#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	constexpr std::string_view DEFAULT_SHADER_LOCATION = RESOURCE_DIR "/shaders/default.shader";

	struct MIKU_API ShaderUniform
	{
		std::string Name;
		unsigned int Index;
		unsigned int Type;
	};

	class MIKU_API Shader : public Asset
	{
	public:
		Shader() : Asset( AssetType::SHADER ) {};
		Shader( UUID uuid, const std::filesystem::path& path );

		void ParseShader( std::string_view filepath, std::string& vs, std::string& gs, std::string& fs );

		unsigned int CompileShader( const std::string& source, unsigned int type );
		unsigned int CreateShader( const std::string& vs, const std::string& gs, const std::string& fs );

		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& path );

		void LoadFromFile( const std::filesystem::path& filepath );
		void PrepareUniforms();

		void Bind() const;
		void UnBind() const;

		const std::filesystem::path& GetPath() const;
		const UUID& GetUUID() const { return m_ShaderUUID; }

		std::string GetName() const;

		void RenderInspectorImGui() override;

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
			static_assert( sizeof( T ) == 0, "No specialization of this type!" );
		}

		void DeleteAsset() override;

	private:
		unsigned int m_RendererID = 0;
		UUID m_ShaderUUID;

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
