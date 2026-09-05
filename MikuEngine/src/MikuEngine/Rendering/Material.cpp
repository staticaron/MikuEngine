#include "Rendering/Material.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Application.h"
#include "Helpers/SerializationHelper.h"
#include "Logger.h"

namespace MikuEngine
{
	Material::Material( UUID uuid, const std::filesystem::path& materialPath ) : Asset( AssetType::MATERIAL ), m_UUID( uuid )
	{
		LoadFromFile( materialPath );
	}

	void Material::CreateFromShader( const UUID& uuid )
	{
		SetShader( uuid );
	}

	void Material::RefreshUniforms()
	{
		m_Textures.clear();
		m_Floats.clear();
		m_Vec2s.clear();
		m_Vec4s.clear();
		m_Mat4s.clear();
		m_Cubemaps.clear();

		const auto& shader = GetShader();

		for ( const auto& [ x, y ] : shader.value()->shader.GetUniforms() )
		{
			switch ( y.Type )
			{
			case GL_SAMPLER_2D: {
				m_Textures[ y.Name ] = 0;
				break;
			}
			case GL_SAMPLER_CUBE: {
				m_Cubemaps[ y.Name ] = 0;
				break;
			}
			case GL_FLOAT: {
				m_Floats[ y.Name ] = 0.0f;
				break;
			}
			case GL_FLOAT_VEC2: {
				m_Vec2s[ y.Name ] = glm::vec2{};
				break;
			}
			case GL_FLOAT_VEC4: {
				m_Vec4s[ y.Name ] = glm::vec4{};
				break;
			}
			case GL_FLOAT_MAT4: {
				m_Mat4s[ y.Name ] = glm::mat4{ 1.0f };
				break;
			}
			default:
				break;
			}
		}
	}

	void Material::Refresh()
	{
		if ( m_Shader.value() == false ) return;

		CreateFromShader( m_Shader.value() );
	}

	void Material::LoadFromFile( const std::filesystem::path& materialPath )
	{
		const ShaderManager& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		YAML::Node rootNode = YAML::LoadFile( materialPath );

		auto shaderUUID = rootNode[ "shader" ].as<std::string>();
		if ( shaderUUID == "<NONE>" ) return;
		SetShader( shaderManager.GetShader( shaderUUID ).index.uuid );

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

			// If this is a blend mode property, set the blend mode and continue with next properties
			if ( name == "blend_mode" )
			{
				auto blendMode = it->second.as<std::string>();
				if ( blendMode == "TRANSPARENT" ) m_RenderOrder.mode = MaterialBlendMode::TRANSPARENT;
				if ( blendMode == "OPAQUE" ) m_RenderOrder.mode = MaterialBlendMode::OPAQUE;
				continue;
			}

			if ( name == "render_order" )
			{
				auto renderOrder = it->second.as<uint32_t>();
				m_RenderOrder.order = renderOrder;
				continue;
			}

			if ( tag == "!tex" )
			{
				UUID uuid( it->second.as<std::string>() );
				m_Textures[ name ] = uuid;
			}
			else if ( tag == "!cube" )
			{
				auto uuidStr = it->second.as<std::string>();
				UUID uuid( uuidStr );
				m_Cubemaps[ name ] = uuid;
			}
			else if ( tag == "!vec2" )
			{
				glm::vec2 value;
				DecodeVec2( it->second, value );
				m_Vec2s[ name ] = value;
			}
			else if ( tag == "!float" )
			{
				float value = it->second.as<float>();
				m_Floats[ name ] = value;
			}

			m_UniformOrder.push_back( name );
		}
	}

	void Material::SaveToFile( const std::filesystem::path& filepath )
	{
		const auto shader = GetShader();

		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << ( shader.has_value() ? shader.value()->shader.GetUUID().ToString() : "<NONE>" );
		emitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

		emitter << YAML::Key << "blend_mode" << YAML::Value << GetBlendModeString();
		emitter << YAML::Key << "render_order" << YAML::Value << GetRenderOrder().order;

		for ( const auto& [ name, value ] : m_Textures )
			emitter << YAML::Key << name << YAML::LocalTag( "tex" ) << YAML::Value << value.ToString();

		for ( const auto& [ name, value ] : m_Cubemaps )
			emitter << YAML::Key << name << YAML::LocalTag( "cube" ) << YAML::Value << value.ToString();

		for ( const auto& [ name, value ] : m_Vec2s )
			emitter << YAML::Key << name << YAML::LocalTag( "vec2" ) << YAML::Flow << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;

		for ( const auto& [ name, value ] : m_Floats )
			emitter << YAML::Key << name << YAML::LocalTag( "float" ) << YAML::Value << value;

		emitter << YAML::EndMap;
		emitter << YAML::EndMap;

		std::filesystem::path pathToSave = filepath;
		pathToSave = pathToSave.remove_filename().string() + GetName() + ".mat";

		std::ofstream fout( pathToSave );
		fout << emitter.c_str();
	}

	void Material::Bind()
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();

		auto shader = GetShader();

		if ( shader.has_value() == false ) return;

		shader.value()->shader.Bind();

		// Handle Floats
		for ( const auto& [ name, value ] : m_Floats )
			shader.value()->shader.SetUniform<float>( name, value );

		// Handle Vec2s
		for ( const auto& [ name, value ] : m_Vec2s )
			shader.value()->shader.SetUniform<glm::vec2>( name, value );

		// Handle Textures
		unsigned int textureID = 1;

		for ( const auto& [ name, uuid ] : m_Textures )
		{
			// Ignore the textures uniforms with no Bound Values
			if ( uuid == 0 ) continue;
			auto texture = textureManager.GetTextureOrDefault( uuid );

			texture->Bind( textureID );
			shader.value()->shader.SetUniform<unsigned int>( name, textureID );

			textureID++;
		}

		for ( const auto& [ name, uuid ] : m_Cubemaps )
		{
			// Ignore the textures uniforms with no Bound Values
			if ( uuid == 0 ) continue;

			auto cubemap = textureManager.GetCubemap( uuid );

			cubemap.value()->Bind( textureID );
			shader.value()->shader.SetUniform<unsigned int>( name, textureID );

			textureID++;
		}
	}

	void Material::UnBind()
	{
		GetShader().value()->shader.UnBind();
	}

	const std::filesystem::path& Material::GetPath() const
	{
		auto material = Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().GetMaterial( m_UUID );
		MIKU_ASSERT( material.has_value(), "This Shader with UUID doesn't exists!" );
		return material.value()->index.path;
	}

	std::string Material::GetName() const
	{
		return GetPath().stem().string();
	}

	void Material::SetName( const std::string& newName )
	{
		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().RenameAsset( m_UUID, newName );
	}

	std::optional<ShaderContainer*> Material::GetShader()
	{
		if ( m_Shader.has_value() == false )
		{
			return std::nullopt;
		}

		auto value = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().GetShader( m_Shader.value() );

		if ( value.has_value() == false )
		{
			// Empty the shader UUID container and the loaded properties
			m_Shader = std::nullopt;
			m_Textures.clear();
			m_Floats.clear();
			m_Mat4s.clear();
			m_Vec4s.clear();

			return std::nullopt;
		}

		return value;
	}

	void Material::SetShader( const UUID& uuid )
	{
		m_Shader = uuid;
		RefreshUniforms();
	}

	std::string Material::GetBlendModeString() const
	{
		switch ( m_RenderOrder.mode )
		{
		case MaterialBlendMode::TRANSPARENT:
			return "TRANSPARENT";
		default:
			return "OPAQUE";
		}
	}

	void Material::DeleteAsset()
	{
		MIKU_CORE_WARN( "Deleting Material!!" );
		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().AddToDeleteQueue( m_UUID );
	}

	void Material::RegisterUniform( std::string, ShaderUniform shaderUniform )
	{
		switch ( shaderUniform.Type ) {}
	}
}
