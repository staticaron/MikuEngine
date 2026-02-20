#include "Rendering/Material.h"

#include <fstream>

#include "Logger.h"
#include "UUID.h"
#include "yaml-cpp/yaml.h"

namespace MikuEngine
{
	void Material::Init( const std::string& materialPath )
	{
		MIKU_CLIENT_INFO( "Material Location : {}", materialPath );

		YAML::Node rootNode = YAML::LoadFile( materialPath );

		m_Shader = rootNode[ "shader" ].as<std::string>();

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

			if ( tag == "!id" )
			{
				UUID uuid( it->second.as<std::string>() );
				m_UUIDs[ name ] = uuid;
			}
			else if ( tag == "!float" )
			{
				float value = it->second.as<float>();
				m_Floats[ name ] = value;
			}
		}
	}

	void Material::Serialize( const char* filepath )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << m_Shader.ToString();
		emitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

		for ( const auto& [ name, value ] : m_UUIDs )
			emitter << YAML::Key << name << YAML::LocalTag( "id" ) << YAML::Value << value.ToString();

		for ( const auto& [ name, value ] : m_Floats )
			emitter << YAML::Key << name << YAML::LocalTag( "float" ) << YAML::Value << value;

		emitter << YAML::EndMap;
		emitter << YAML::EndMap;

		std::ofstream fout( filepath );
		fout << emitter.c_str();
	}

	void Material::Bind() {}

	void Material::UnBind() {}
}
