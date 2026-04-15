#include "Rendering/Material.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Application.h"
#include "Logger.h"
#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	Material::Material( UUID uuid, const std::filesystem::path& materialPath ) : Asset( AssetType::MATERIAL ), m_UUID( uuid )
	{
		LoadFromFile( materialPath );
	}

	void Material::CreateFromShader( const UUID& uuid )
	{
		m_Textures.clear();
		m_Floats.clear();
		m_Vec4s.clear();
		m_Mat4s.clear();

		const auto& shader = GetShader();

		for ( const auto& [ x, y ] : shader.value()->GetUniforms() )
		{
			switch ( y.Type )
			{
			case GL_SAMPLER_2D: {
				m_Textures[ y.Name ] = 0;
				break;
			}
			case GL_FLOAT: {
				m_Floats[ y.Name ] = 0.0f;
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
		YAML::Node rootNode = YAML::LoadFile( materialPath );

		const ShaderManager& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

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

			m_UniformOrder.push_back( name );
		}
	}

	void Material::SaveToFile( const std::filesystem::path& filepath )
	{
		const auto shader = GetShader();

		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << ( shader.has_value() ? shader.value()->GetUUID().ToString() : "<NONE>" );
		emitter << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;

		for ( const auto& [ name, value ] : m_Textures )
			emitter << YAML::Key << name << YAML::LocalTag( "tex" ) << YAML::Value << value.ToString();

		for ( const auto& [ name, value ] : m_Floats )
			emitter << YAML::Key << name << YAML::LocalTag( "float" ) << YAML::Value << value;

		emitter << YAML::EndMap;
		emitter << YAML::EndMap;

		std::filesystem::path pathToSave = filepath;
		pathToSave = pathToSave.remove_filename().string() + GetName() + ".mat";

		std::ofstream fout( pathToSave );
		fout << emitter.c_str();
	}

	void Material::CreateAssetAtPath( const std::string& name, const std::filesystem::path& path )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "shader" << YAML::Value << "<NONE>";
		emitter << YAML::EndMap;

		unsigned int count = 0;
		std::filesystem::path pathToSave = path / ( name + ".mat" );

		while ( std::filesystem::exists( pathToSave ) )
		{
			count++;
			pathToSave = path / ( name + "_" + std::to_string( count ) + ".mat" );
		}

		std::ofstream fout( pathToSave );
		fout << emitter.c_str();
		fout.close();

		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().Refresh();
	}

	void Material::Bind()
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();

		auto shader = GetShader();

		if ( shader.has_value() == false ) return;

		shader.value()->Bind();

		// Handle Floats
		for ( const auto& [ name, value ] : m_Floats )
			shader.value()->SetUniform<float>( name, value );

		// Handle Textures
		unsigned int textureID = 0;

		for ( const auto& [ name, uuid ] : m_Textures )
		{
			// Ignore the textures uniforms with no Bound Values
			if ( uuid == 0 ) continue;
			const auto& textureContainer = textureManager.GetTexture( uuid );

			if ( textureContainer.has_value() == false ) continue;

			textureContainer.value()->texture.Bind( textureID );
			shader.value()->SetUniform<unsigned int>( name, textureID );

			textureID++;
		}
	}

	void Material::UnBind()
	{
		GetShader().value()->UnBind();
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

	std::optional<Shader*> Material::GetShader()
	{
		if ( m_Shader.has_value() == false )
		{
			MIKU_CORE_ERROR( "This material has no Shader!" );
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

		return &value.value()->shader;
	}

	void Material::SetShader( const UUID& uuid )
	{
		m_Shader = uuid;
	}

	void Material::AssetImGui()
	{
		auto& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		// Render Material Details
		auto materialName = GetName();
		char materialNameBuff[ 256 ];
		std::copy( materialName.begin(), materialName.begin() + materialName.size(), materialNameBuff );
		materialNameBuff[ materialName.size() ] = '\0';

		DISABLED_IMGUI( ImGui::InputText( "##MaterialName", materialNameBuff, 256 ) );
		ImGui::Separator();
		ImGui::Separator();

		const auto& shader = GetShader();

		// Render Shader Details
		auto shaderName = shader.has_value() ? shader.value()->GetName() : "<NONE>";
		char shaderNameBuff[ 256 ];
		std::copy( shaderName.begin(), shaderName.begin() + shaderName.size(), shaderNameBuff );
		shaderNameBuff[ shaderName.size() ] = '\0';

		ImGui::InputText( "Shader", shaderNameBuff, 256, ImGuiInputTextFlags_ReadOnly );

		if ( ImGui::BeginDragDropTarget() )
		{
			auto payload = ImGui::AcceptDragDropPayload( "SHADER_DRAG_DROP_PAYLOAD" );

			if ( payload != nullptr )
			{
				std::string shaderFilePath = static_cast<const char*>( payload->Data );
				SetShader( shaderManager.GetShaderByFilePath( shaderFilePath ).index.uuid );

				Refresh();
			}

			ImGui::EndDragDropTarget();
		}

		// If no shader is attached then no need to render the shader properties
		if ( shader.has_value() )
		{
			// Render Textures
			for ( auto& [ uniformName, uuid ] : m_Textures )
			{
				const auto& textureUUID = m_Textures[ uniformName ];

				auto texture = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTexture( uuid );

				std::string textureName = "<NONE>";
				if ( texture.has_value() ) textureName = texture.value()->GetName();

				char buff[ 256 ] = "";
				std::copy( textureName.begin(), textureName.end(), buff );
				buff[ textureName.length() ] = '\0';

				ImGui::InputText( uniformName.c_str(), buff, 256, ImGuiInputTextFlags_ReadOnly );

				if ( ImGui::BeginDragDropTarget() )
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

					if ( payload != nullptr )
					{
						std::string filePath = static_cast<const char*>( payload->Data );
						std::filesystem::path materialPath = filePath;

						const auto& texture = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( filePath );
						m_Textures[ uniformName ] = texture.value()->index.uuid;
					}

					ImGui::EndDragDropTarget();
				}
			}

			// Render Floats
			for ( auto& [ uniformName, value ] : m_Floats )
			{
				ImGui::DragFloat( uniformName.c_str(), &m_Floats[ uniformName ] );
			}
		}

		// Render Save Material Button
		ImGui::Separator();
		if ( ImguiManager::FullWidthButton( "SAVE" ) )
		{
			SaveToFile( GetPath() );
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
