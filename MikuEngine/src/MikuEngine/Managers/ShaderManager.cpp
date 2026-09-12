#include "Managers/ShaderManager.h"

#include <filesystem>
#include <fstream>

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void ShaderManager::Init()
	{
		LoadShaderIncludes();
		LoadAllDefaultShaders( true );
		LoadAllProjectShaders( true );
	}

	void ShaderManager::UnloadShader( const std::filesystem::path& filepath )
	{
		auto shader = m_Shaders.begin();

		// Find the iterator with same filepath
		while ( shader != m_Shaders.end() )
		{
			if ( shader->second.GetPath() == filepath )
				break;
			shader++;
		}

		// Shader not loaded! Can't unload
		if ( shader == m_Shaders.end() )
		{
			MIKU_CORE_DEBUG( "Tried to Remove Shader named {} but no shader of that name was found!", filepath.stem().c_str() );
			return;
		}

		// Delete the shader
		m_Shaders.erase( shader );
		MetaFileManager::DeleteMetaFile( filepath );

		MIKU_CORE_DEBUG( "[UNLOADED] Shader Removed named {}", filepath.stem().c_str() );
	}

	void ShaderManager::InitFrame()
	{
		PerformDeletions();
		PerformRenames();
	}

	/// @brief Load Shader with the given UUID and shader source file
	/// @param uuid UUID of the shader object
	/// @param filepath path to the source file
	void ShaderManager::LoadShader( const std::filesystem::path& filepath, UUID uuid )
	{
		Shader shader( uuid, filepath );
		m_Shaders[ uuid ] = { shader };
	}

	/// @brief Load Default Shader with the given UUID and shader source file
	/// @param uuid UUID of the shader object
	/// @param filepath path to the source file
	void ShaderManager::LoadDefaultShader( const std::filesystem::path& filepath, UUID uuid )
	{
		Shader shader( uuid, filepath );
		m_DefaultShaders[ uuid ] = { shader };
	}

	/// @brief Load all the default shaders
	void ShaderManager::LoadAllDefaultShaders( bool loadExisting )
	{
		int numberOfShadersLoaded{ 0 };

		for ( auto& item : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/shaders/" ) )
		{
			if ( item.path().extension() != ".shader" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::SHADER, GetShaderProperties( nullptr ) );

			if ( loadExisting )
			{
				LoadDefaultShader( item.path(), uuid );
				numberOfShadersLoaded++;
			}
			else
			{
				if ( auto existing = m_DefaultShaders.find( uuid ); existing != m_DefaultShaders.end() )
					continue; // Skip if the shader is already loaded

				LoadDefaultShader( item.path(), uuid );
				numberOfShadersLoaded++;
			}
		}

		MIKU_CORE_INFO( "Number of Default Shaders Loaded : {}", numberOfShadersLoaded );
	}

	/// @brief Load all the shader includes
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

	/// Load all the shaders
	///
	/// @param loadExisting if true, shader will be loaded and existing shader with same UUID will be replaced
	///
	void ShaderManager::LoadAllProjectShaders( bool loadExisting )
	{
		const auto& dataContainer = Application::GetDataContainer();

		for ( auto& item : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/shaders/" ) ) )
		{
			if ( item.path().extension() != ".shader" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::SHADER, GetShaderProperties( nullptr ) );

			if ( loadExisting )
				LoadShader( item.path(), uuid );
			else
			{
				if ( auto existing = m_Shaders.find( uuid ); existing != m_Shaders.end() )
					continue; // Skip if existing shader is found with the UUID

				LoadShader( item.path(), uuid );
			}
		}

		MIKU_CORE_DEBUG( "{} Shaders Loaded!", m_Shaders.size() );
	}

	/// @brief Go through the shader directories and reload the ones not loaded yet
	void ShaderManager::RefreshShaders()
	{
		LoadAllProjectShaders( false );
		LoadAllDefaultShaders( false );
	}

	/// @brief Return the path of the shader currently loaded in the DB
	/// @param uuid uuid of the shader to get the filepath of
	const std::filesystem::path& ShaderManager::GetFilePathByUUID( const UUID& uuid )
	{
		if ( auto shader = m_Shaders.find( uuid ); shader != m_Shaders.end() )
			return shader->second.GetPath();

		MIKU_ASSERT( false, "This shader is not loaded! Can't return FilePath" );
	}

	/// @brief Finds a shader in the loaded projects shaders and default shaders
	/// @param shaderUUID UUID of the shader to search
	/// @return shader pointer or nullptr if the shader is not found
	Shader* ShaderManager::GetShader( UUID shaderUUID )
	{
		return const_cast<Shader*>( static_cast<const ShaderManager*>( this )->GetShader( shaderUUID ) );
	}

	/// @brief Finds a shader in the loaded projects shaders and default shaders
	/// @param shaderUUID UUID of the shader to search
	/// @return shader pointer or nullptr if the shader is not found
	const Shader* ShaderManager::GetShader( UUID shaderUUID ) const
	{
		if ( auto existing = m_Shaders.find( shaderUUID ); existing != m_Shaders.end() )
			return &existing->second;

		if ( auto existing = m_DefaultShaders.find( shaderUUID ); existing != m_DefaultShaders.end() )
			return &existing->second;

		return nullptr;
	}

	/// Searches for the shader and returns the project shader, default shader with UUID or just a defualt shader in order if the not found previously
	///
	/// @param shaderUUID UUID of the shader to fetch
	const Shader* ShaderManager::GetShaderOrDefault( UUID shaderUUID ) const
	{
		if ( shaderUUID == UUID( 0 ) )
			return GetDefaultShader();

		// Return the loaded shader otherwise the deafult shader
		if ( auto shader = GetShader( shaderUUID ); shader != nullptr )
			return shader;

		if ( auto defaultShader = GetDefaultShader( shaderUUID ); defaultShader != nullptr )
			return defaultShader;

		return GetDefaultShader();
	}

	/// @brief Finds a default shader in the loaded default shaders
	/// @param shaderUUID UUID of the default shader to search
	/// @return shader pointer or nullptr if the default shader is not found
	const Shader* ShaderManager::GetDefaultShader( UUID shaderUUID ) const
	{
		if ( auto existing = m_DefaultShaders.find( shaderUUID ); existing != m_DefaultShaders.end() )
			return &existing->second;

		return nullptr;
	}

	/// Search a shader by its name
	/// @param name name of the shader
	/// @return shader pointer or nullptr is shader is not found
	Shader* ShaderManager::GetShaderByName( const std::string& name )
	{
		for ( auto& [ uuid, shader ] : m_Shaders )
		{
			if ( shader.GetName() == name )
				return &shader;
		}

		return nullptr;
	}

	/// @brief Search a shader by its path
	/// @param path path of the shader to find
	/// @return shader pointer or nullptr is shader is not found
	Shader* ShaderManager::GetShaderByFilePath( const std::filesystem::path& path )
	{
		for ( auto& [ uuid, shader ] : m_Shaders )
		{
			if ( shader.GetPath() == path )
				return &shader;
		}

		return nullptr;
	}

	std::string ShaderManager::GetShaderName( UUID textureUUID ) const
	{
		auto existing = m_Shaders.find( textureUUID );
		return existing->second.GetName();
	}

	/// Add a shader to the delete queue
	///
	/// @param uuid UUID of the shader
	void ShaderManager::AddToDeleteQueue( const UUID& uuid )
	{
		m_DeleteQueue.push_back( uuid );
	}

	/// Add shader by filepath to the delete queue
	///
	/// @param filepath the filepath of the shader to be deleted
	void ShaderManager::AddToDeleteQueue( const std::filesystem::path& filepath )
	{
		if ( auto existing = GetShaderByFilePath( filepath ); existing != nullptr )
			m_DeleteQueue.push_back( existing->GetUUID() );
	};

	void ShaderManager::AddToRenameQueue( const UUID& uuid, const std::string& newName )
	{
		m_RenameQueue.push_back( { uuid, newName } );
	};

	void ShaderManager::AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName )
	{
		if ( auto existing = GetShaderByFilePath( filepath ); existing != nullptr )
			m_RenameQueue.push_back( { existing->GetUUID(), newName } );
	};

	bool ShaderManager::ShaderExists( const UUID& uuid ) const
	{
		auto exists = m_Shaders.find( uuid );
		return exists != m_Shaders.end();
	}

	/// Return the first default shader
	/// Use this as a fallback shader
	///
	/// @return Shader pointer to a default shader
	const Shader* ShaderManager::GetDefaultShader() const
	{
		return &m_DefaultShaders.begin()->second;
	}

	const std::string& ShaderManager::GetShaderIncludeCode( const std::string& identifier ) const
	{
		return m_ShaderIncludes.at( identifier );
	}

	/// Get Meta File data for a Shader
	///
	/// @param shader a pointer to the shader object! Pass nullptr to get generic data
	YAML::Node ShaderManager::GetShaderProperties( Shader* shader )
	{
		return {};
	}

	/// @brief Create a Sample Shader at the provided location
	/// @param name name of the shader
	/// @param folderPath path of the parent folder where the shader will be created
	void ShaderManager::CreateAssetAtPath( const std::string& name, const std::filesystem::path& folderPath )
	{
		// Find a proper name for the new shader file by appending a number until a new name is found
		unsigned int count = 0;
		std::filesystem::path pathToSave = folderPath / ( name + ".shader" );

		while ( std::filesystem::exists( pathToSave ) )
		{
			count++;
			pathToSave = folderPath / ( name + "_" + std::to_string( count ) + ".shader" );
		}

		// Copy the default shader file from engine to the project
		std::filesystem::copy( DEFAULT_2D_SHADER_LOCATION, pathToSave );

		// Load the shader asset into the shader DB
		Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().LoadShader( pathToSave, {} );

		MetaFileManager::GenerateMetaFileIfNotPresent( pathToSave, AssetType::SHADER, ShaderManager::GetShaderProperties() );
	}

	/// @brief Go through each and every UUID in delete queue and perform delete on the asset with that UUID
	void ShaderManager::PerformDeletions()
	{
		if ( m_DeleteQueue.size() <= 0 )
			return;

		for ( auto& x : m_DeleteQueue )
			DeleteAsset( x );

		MIKU_CORE_INFO( "Deleted {} Shaders", m_DeleteQueue.size() );

		m_DeleteQueue.clear();
	}

	/// @brief Go through each and every UUID in rename queue and perform rename on the asset with that UUID
	void ShaderManager::PerformRenames()
	{
		if ( m_RenameQueue.size() <= 0 )
			return;

		for ( auto& x : m_RenameQueue )
			RenameAsset( x.first, x.second );

		MIKU_CORE_INFO( "Renamed {} Shaders", m_RenameQueue.size() );

		m_RenameQueue.clear();
	}

	/// @brief Unload the Shader and Remove the Shader Files
	/// @param uuid UUID of the shader to delete
	void ShaderManager::DeleteAsset( const UUID& uuid )
	{
		auto shaderToDelete = m_Shaders.find( uuid );

		if ( shaderToDelete == m_Shaders.end() )
			return;

		auto filepath = shaderToDelete->second.GetPath();

		// Free the GPU Memory
		shaderToDelete->second.Destroy();

		// Remove from the Shader DB
		m_Shaders.erase( shaderToDelete );

		// Delete the physical files
		if ( std::filesystem::exists( filepath ) )
			std::filesystem::remove( filepath );
		if ( std::filesystem::exists( filepath.string() + ".meta" ) )
			std::filesystem::remove( filepath.string() + ".meta" );
	}

	/// @brief Rename the Shader by updating the filepath in shader object and renaming the shader asset file
	/// @param uuid UUID of the shader to rename
	/// @param newName new name of the shader asset
	void ShaderManager::RenameAsset( const UUID& uuid, const std::string& newName )
	{
		auto shaderToRename = m_Shaders.find( uuid );

		if ( shaderToRename == m_Shaders.end() )
			return;

		const std::filesystem::path filePath = GetFilePathByUUID( uuid );
		const std::string fileExtension = filePath.extension();

		// Rename the asset files
		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		// Update the filepath in the shader object
		shaderToRename->second.SetPath( newFilePath );
	}
}
