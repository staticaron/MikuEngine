#include "Managers/ShaderManager.h"

#include <filesystem>
#include <fstream>

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void ShaderManager::LoadShader( const std::string& name, const std::string& filepath )
	{
		UUID newUUID;
		Shader shader( newUUID, filepath );

		m_Shaders[ newUUID ] = {
		    { name, filepath },
			shader
		       };
	}

	void ShaderManager::LoadDefaultShaders()
	{
		for ( auto [ uuid, shaderIndex ] : m_DefaultShaderIndex )
		{
			Shader shader( uuid, shaderIndex.path );
			m_DefaultShaders[ uuid ] = { shaderIndex, shader };
		}
	}

	void ShaderManager::LoadShaderIncludes()
	{
		for ( auto file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/shaders/includes/" ) )
		{
			if ( file.path().extension() != ".inc" )
				continue;

			std::ifstream contentFile( file.path().string() );

			if ( !contentFile.is_open() )
			{
				MIKU_CORE_WARN( "Unable to load {}! Shaders might not work correctly!", file.path().filename().string() );
				return;
			}

			std::stringstream buffer;
			buffer << std::ifstream( file.path().string().c_str() ).rdbuf();

			m_ShaderIncludes.insert( { file.path().stem().string(), buffer.str() } );
		}
	}

	void ShaderManager::LoadAllShaders()
	{
		PrepareShaderIndex();
		LoadShaderIncludes();
		LoadDefaultShaders();

		for ( auto [ uuid, shaderIndex ] : m_ShaderIndex )
		{
			Shader shader( uuid, shaderIndex.path );
			m_Shaders[ uuid ] = { shaderIndex, shader };
		}

		MIKU_CORE_DEBUG( "{} Shaders Loaded!", m_Shaders.size() );
	}

	void ShaderManager::Refresh()
	{
		PrepareShaderIndex();
		RefreshShaders();
	}

	void ShaderManager::RefreshShaders()
	{
		unsigned int refreshCount = 0;

		for ( const auto& [ uuid, index ] : m_ShaderIndex )
		{
			const auto& existing = m_Shaders.find( uuid );

			if ( existing != m_Shaders.end() )
				continue;

			Shader shader( uuid, index.path );
			m_Shaders[ uuid ] = { index, shader };

			refreshCount++;
		}
	}

	const std::filesystem::path& ShaderManager::GetFilePathFromUUID( const UUID& uuid )
	{
		if ( auto shaderContainer = m_Shaders.find( uuid ); shaderContainer != m_Shaders.end() )
		{
			return shaderContainer->second.index.path;
		}

		MIKU_ASSERT( false, "This shader is not loaded!" );
	}

	void ShaderManager::RenameAssetCleanup( const UUID& uuid, const std::string& newName )
	{
		if ( auto existing = m_Shaders.find( uuid ); existing != m_Shaders.end() )
		{
			existing->second.SetName( newName );
		}
	}

	void ShaderManager::DeleteAssetCleanup( const UUID& uuid )
	{
		if ( const auto& existing = m_Shaders.find( uuid ); existing != m_Shaders.end() )
		{
			m_Shaders.erase( existing );
		}
	}

	void ShaderManager::PrepareShaderIndex()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/shaders/" ) )
		{
			if ( file.path().extension() != ".shader" )
				continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
				MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::SHADER, GetShaderProperties( nullptr ) );

			if ( file.is_directory() )
				continue;

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_DefaultShaderIndex[ uuid ] = { uuid, file.path().string() };
		}

		const auto& dataContainer = Application::GetDataContainer();
		for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/shaders/" ) ) )
		{
			if ( file.path().extension() != ".shader" )
				continue;

			if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
				MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::SHADER, GetShaderProperties( nullptr ) );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_ShaderIndex[ uuid ] = { uuid, file.path().string() };
		}

		MIKU_CORE_DEBUG( "Shader Indexing Complete!", m_ShaderIndex.size() );
	}

	const std::unordered_map<UUID, ShaderIndexEntry>& ShaderManager::GetShaderIndex() const
	{
		return m_ShaderIndex;
	}

	std::optional<ShaderContainer*> ShaderManager::GetShader( UUID shaderUUID )
	{
		if ( auto existing = m_Shaders.find( shaderUUID ); existing != m_Shaders.end() )
			return &existing->second;
		else
		{
			if ( auto existing = m_DefaultShaders.find( shaderUUID ); existing != m_Shaders.end() )
				return &existing->second;
			return std::nullopt;
		}
	}

	const ShaderContainer& ShaderManager::GetShader( UUID shaderUUID ) const
	{
		if ( shaderUUID == UUID( 0 ) )
			return GetDefaultShader();

		// Return the loaded shader otherwise the deafult shader
		if ( auto existing = m_Shaders.find( shaderUUID ); existing != m_Shaders.end() )
			return existing->second;
		if ( auto existing = m_DefaultShaders.find( shaderUUID ); existing != m_Shaders.end() )
			return existing->second;

		return GetDefaultShader();
	}

	ShaderContainer& ShaderManager::GetShaderByName( const std::string& name )
	{
		for ( auto& [ uuid, shaderContainer ] : m_Shaders )
		{
			if ( shaderContainer.GetName() == name )
				return shaderContainer;
		}

		MIKU_ASSERT( false, "Requested Shader is not loaded!" );
	}

	std::optional<const ShaderContainer*> ShaderManager::GetShaderByFilePath( const std::filesystem::path& path )
	{
		for ( const auto& [ uuid, shaderContainer ] : m_Shaders )
		{
			if ( shaderContainer.index.path == path )
				return &m_Shaders.at( uuid );
		}

		return {};
	}

	bool ShaderManager::ShaderExists( const UUID& uuid ) const
	{
		auto exists = m_Shaders.find( uuid );
		return exists != m_Shaders.end();
	}

	std::string ShaderManager::GetShaderName( UUID textureUUID ) const
	{
		auto existing = m_Shaders.find( textureUUID );
		return existing->second.GetName();
	}

	const ShaderContainer& ShaderManager::GetDefaultShader() const
	{
		return m_DefaultShaders.begin()->second;
	}

	const std::string& ShaderManager::GetShaderIncludeCode( const std::string& identifier ) const
	{
		return m_ShaderIncludes.at( identifier );
	}

	YAML::Node ShaderManager::GetShaderProperties( Shader* shader )
	{
		return {};
	}

	void ShaderManager::CreateAssetAtPath( const std::string& name, const std::filesystem::path& folderPath )
	{
		unsigned int count = 0;
		std::filesystem::path pathToSave = folderPath / ( name + ".shader" );

		while ( std::filesystem::exists( pathToSave ) )
		{
			count++;
			pathToSave = folderPath / ( name + "_" + std::to_string( count ) + ".shader" );
		}

		std::filesystem::copy( DEFAULT_2D_SHADER_LOCATION, pathToSave );

		Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().Refresh();
	}
}
