#include "Rendering/Material.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Application.h"
#include "Logger.h"
#include "UUID.h"

namespace MikuEngine
{
	void Material::CreateFromShader( const Shader& shader )
	{
		auto uniforms = shader.GetUniforms();

		for ( auto [ name, uniform ] : uniforms )
		{
			if ( uniform.Type == GL_FLOAT )
				m_Floats[ name ] = 0.0f;
			else if ( uniform.Type == GL_SAMPLER_2D )
				m_Floats[ name ] = 0.0f;
		}
	}

	void Material::LoadFromFile( const std::string& materialPath )
	{
		MIKU_CLIENT_INFO( "Material Location : {}", materialPath );

		YAML::Node rootNode = YAML::LoadFile( materialPath );

		const ShaderManager& shaderManager = Application::GetAppLevelStuff().GetShaderManager();

		m_ShaderID = UUID( rootNode[ "shader" ].as<std::string>() );
		auto m_Shader = shaderManager.GetShader( m_ShaderID ).shader;

		YAML::Node paramterNodes = rootNode[ "properties" ];

		if ( paramterNodes.IsMap() == false )
		{
			MIKU_CLIENT_ERROR( "Parameter Node is not a sequence of values" );
			return;
		}

		for ( YAML::const_iterator it = paramterNodes.begin(); it != paramterNodes.end(); ++it )
		{
			auto name = it->first.as<std::string>();
			auto tag = it->second.Tag();

			if ( tag == "!tex" )
			{
				UUID uuid( it->second.as<std::string>() );
				m_Textures[ name ] = uuid;
			}
			else if ( tag == "!float" )
			{
				float value = it->second.as<float>();
				m_Floats[ name ] = value;
			}
		}
	}

	void Material::SaveToFile( const char* filepath ) const
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << m_ShaderID.ToString();
		emitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

		for ( const auto& [ name, value ] : m_Textures )
			emitter << YAML::Key << name << YAML::LocalTag( "tex" ) << YAML::Value << value.ToString();

		for ( const auto& [ name, value ] : m_Floats )
			emitter << YAML::Key << name << YAML::LocalTag( "float" ) << YAML::Value << value;

		emitter << YAML::EndMap;
		emitter << YAML::EndMap;

		std::ofstream fout( filepath );
		fout << emitter.c_str();
	}

	void Material::Bind()
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetTextureManager();
		m_Shader.Bind();

		// Handle Floats
		for ( const auto& [ name, value ] : m_Floats )
			m_Shader.SetUniform<float>( name, value );

		// Handle Textures
		unsigned int textureID = 0;

		for ( const auto& [ name, uuid ] : m_Textures )
		{
			auto texture = textureManager.GetTexture( uuid );
			texture.Bind( textureID++ );
			m_Shader.SetUniform<unsigned int>( name, textureID );
		}
	}

	void Material::UnBind()
	{
		m_Shader.UnBind();
	}
}
