#include "Rendering/Material.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Application.h"
#include "Logger.h"
#include "Managers/ImguiManager.h"
#include "UUID.h"

namespace MikuEngine
{
	Material::Material( UUID uuid, const std::filesystem::path& materialPath ) : Asset( AssetType::MATERIAL ), m_UUID( uuid ), m_MaterialPath( materialPath )
	{
		LoadFromFile( materialPath );
	}

	void Material::CreateFromShader( const Shader& shader ) {}

	void Material::LoadFromFile( const std::filesystem::path& materialPath )
	{
		YAML::Node rootNode = YAML::LoadFile( materialPath );

		const ShaderManager& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		m_ShaderID = UUID( rootNode[ "shader" ].as<std::string>() );
		m_Shader = shaderManager.GetShader( m_ShaderID ).shader;

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

	void Material::SaveToFile( const std::filesystem::path& filepath ) const
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

		std::filesystem::path pathToSave = filepath;
		pathToSave = pathToSave.remove_filename().string() + GetName() + ".mat";

		std::ofstream fout( pathToSave );
		fout << emitter.c_str();
	}

	void Material::Bind()
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();
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

	void Material::RenderInspectorImGui()
	{
		const auto& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		// Render Material Details
		auto materialName = GetName();
		char materialNameBuff[ 256 ];
		std::copy( materialName.begin(), materialName.begin() + materialName.size(), materialNameBuff );
		materialNameBuff[ materialName.size() ] = '\0';

		DISABLED_IMGUI( ImGui::InputText( "##MaterialName", materialNameBuff, 256 ) );
		ImGui::Separator();
		ImGui::Separator();

		// Render Shader Details
		auto shaderName = m_Shader.GetName();
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
				m_Shader = shaderManager.GetShaderByFilePath( shaderFilePath ).shader;
				CreateFromShader( m_Shader );
			}

			ImGui::EndDragDropTarget();
		}

		// Render Textures
		for ( auto& [ uniformName, uuid ] : m_Textures )
		{
			const auto& textureUUID = m_Textures[ uniformName ];
			auto texture = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTextureName( textureUUID );

			char buff[ 256 ] = "";
			std::copy( texture.begin(), texture.end(), buff );
			buff[ texture.length() ] = '\0';

			ImGui::InputText( uniformName.c_str(), buff, 256, ImGuiInputTextFlags_ReadOnly );

			if ( ImGui::BeginDragDropTarget() )
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					std::string filePath = static_cast<const char*>( payload->Data );
					std::filesystem::path materialPath = filePath;

					const auto& texture = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( filePath );
					m_Textures[ uniformName ] = texture->GetUUID();
				}

				ImGui::EndDragDropTarget();
			}
		}

		// Render Floats
		for ( auto& [ uniformName, value ] : m_Floats )
		{
			ImGui::DragFloat( uniformName.c_str(), &m_Floats[ uniformName ] );
		}

		// Render Save Material Button
		ImGui::Separator();
		if ( ImguiManager::FullWidthButton( "SAVE" ) )
		{
			SaveToFile( GetPath() );
		}
	}

	void Material::RegisterUniform( std::string, ShaderUniform shaderUniform )
	{
		switch ( shaderUniform.Type ) {}
	}
}
