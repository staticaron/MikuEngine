#include <filesystem>
#include <unordered_map>

#include "Logger.h"
#include "Managers/MetaFileManager.h"
#include "Rendering/Texture.h"

#include "Application.h"

namespace MikuEngine
{
	void TextureManager::Init()
	{
		LoadAllTextures();
		LoadAllDefaultTextures();
	}

	/// @brief Perform things to prepare the manager for the frame
	void TextureManager::InitFrame()
	{
		PerformDeletions();
	}

	/// @brief Add a texture represented by UUID to the delete queue
	/// @param uuid uuid of the texture to be added to the queue
	void TextureManager::AddToDeleteQueue( const UUID& uuid )
	{
		m_DeleteQueue.push_back( uuid );
	}

	/// @brief Add a texture represented by UUID to the delete queue
	/// @param filepath path to the textur to add to the queue
	void TextureManager::AddToDeleteQueue( const std::filesystem::path& filepath )
	{
		if ( auto texture = GetTextureByFilePath( filepath ); texture.has_value() )
			m_DeleteQueue.push_back( texture.value()->GetUUID() );
	}

	/// @brief Add a texture represented by UUID to the rename queue
	/// @param uuid uuid of the texture to be added to the rename queue
	/// @param newName new name of the texture to be added to the rename queue
	void TextureManager::AddToRenameQueue( const UUID& uuid, const std::string& newName )
	{
		m_RenameQueue.push_back( { uuid, newName } );
	}

	/// @brief Add a texture represented by filepath to the rename queue
	/// @param filepath path to the textur to add to the queue
	/// @param newName new name of the texture to be added to the rename queue
	void TextureManager::AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName )
	{
		if ( auto texture = GetTextureByFilePath( filepath ); texture.has_value() )
			m_RenameQueue.push_back( { texture.value()->GetUUID(), newName } );
	}

	/// @brief Destroy the Texture Manager. Unloads all the loaded textures
	void TextureManager::Destroy()
	{
		for ( auto [ identifier, texture ] : m_Textures )
			texture.Destroy();

		MIKU_CORE_WARN( "Texture Manager Destroyed!" );
	}

	/// @brief Perform Deletion on all the textures added to the Delete Queue
	void TextureManager::PerformDeletions()
	{
		for ( auto item : m_DeleteQueue )
			DeleteAsset( item );

		m_DeleteQueue.clear();
	}

	void TextureManager::PerformRenames()
	{
		for ( auto [ uuid, newName ] : m_RenameQueue )
			RenameAsset( uuid, newName );

		m_RenameQueue.clear();
	}

	/// @brief Delete a texture
	/// @param uuid uuid of the texture to be deleted!
	void TextureManager::DeleteAsset( const UUID& uuid )
	{
		const std::filesystem::path& filePath = GetFilePathByUUID( uuid );

		// Find the texture to delete
		auto textureToDelete = m_Textures.begin();
		for ( ; textureToDelete != m_Textures.end(); textureToDelete++ )
			if ( textureToDelete->first == uuid )
				break;

		// Free the GPU Memory
		textureToDelete->second.Destroy();

		// Remove from the Texture DB
		m_Textures.erase( textureToDelete );

		// Delete the physical files
		if ( std::filesystem::exists( filePath ) )
			std::filesystem::remove( filePath );
		if ( std::filesystem::exists( filePath.string() + ".meta" ) )
			std::filesystem::remove( filePath.string() + ".meta" );
	}

	/// @brief Rename a texture; This includes updating the path as well
	/// @param uuid UUID of the texture to be renamed
	/// @param newName new name of the texture to be renamed
	void TextureManager::RenameAsset( const UUID& uuid, const std::string& newName )
	{
		const std::filesystem::path& filePath = GetFilePathByUUID( uuid );
		const std::string fileExtension = filePath.extension();

		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		if ( auto existing = m_Textures.find( uuid ); existing != m_Textures.end() )
		{
			existing->second.SetName( newName );
		}
	}

	/// @brief Get the path of the texture
	/// @param uuid UUID of the texture
	const std::filesystem::path& TextureManager::GetFilePathByUUID( const UUID& uuid )
	{
		for ( auto texture : m_Textures )
		{
			if ( texture.first == uuid )
				return texture.second.GetPath();
		}

		MIKU_ASSERT( false, "Texture with UUID {} is not loaded!", uuid.ToString() );
	}

	/// @brief Load a texture located a path
	/// @param filepath path of the texture file
	/// @param uuid provide a UUID to use for this texture
	void TextureManager::LoadTexture( const std::filesystem::path& filepath, const UUID& uuid )
	{
		Texture newTexture( uuid, filepath );
		m_Textures.insert( { uuid, newTexture } );
	}

	/// @brief Load a default texture located at path
	/// @param filepath path of the texture file
	/// @param uuid provide a UUID to use for this texture
	void TextureManager::LoadDefaultTexture( const std::filesystem::path& filepath, const UUID& uuid )
	{
		Texture newTexture( uuid, filepath );
		m_DefaultTextures.insert( { uuid, newTexture } );
	}

	/// @brief Go through the files in RESOURCE_DIR/textures/ and for every texture found, get the UUID from meta file and then load the texture
	void TextureManager::LoadAllTextures()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/textures/" ) )
		{
			if ( file.path().extension() == ".meta" )
				continue;
			if ( file.is_directory() )
				continue;

			UUID uuid = MetaFileManager::GenerateMetaFileIfNotPresent( file.path().c_str(), AssetType::TEXTURE, TextureManager::GetTextureProperties( {} ) );
			LoadTexture( file.path(), uuid );
		}

		MIKU_CORE_DEBUG( "All Project Textures loaded!" );

		// TODO: Proper loading of cubemaps
		// Load all the cubemaps
		const auto& dataContainer = Application::GetDataContainer();

		Cubemap cubemap;
		cubemap.LoadFromFile( dataContainer.GetProjectAssetPath( "/textures/skybox/cubemap.jpg" ) );

		UUID uuid( "3583395674924388584" );
		m_Cubemaps[ uuid ] = cubemap;

		MIKU_CORE_DEBUG( "All Cubemaps Loaded!" );
	}

	void TextureManager::LoadAllDefaultTextures()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/textures/" ) )
		{
			if ( file.path().extension() == ".meta" )
				continue;
			if ( file.is_directory() )
				continue;

			UUID uuid = MetaFileManager::GenerateMetaFileIfNotPresent( file.path().c_str(), AssetType::TEXTURE, TextureManager::GetTextureProperties( {} ) );
			LoadDefaultTexture( file.path(), uuid );
		}

		MIKU_CORE_DEBUG( "All Default Textures loaded!" );
	}

	/// @brief Fetch a texture already loaded in the texture manager
	/// @param textureUUID UUID of the texture to fetch
	/// @return an optional mutable pointer to the texture object
	std::optional<Texture*> TextureManager::GetTexture( UUID textureUUID )
	{
		return const_cast<Texture*>( static_cast<TextureManager*>( this )->GetTexture( textureUUID ).value() );
	}

	/// @brief Fetch a texture already loaded in the texture manager
	/// @param textureUUID UUID of the texture to fetch
	/// @return an optional immutable pointer to the texture object
	std::optional<const Texture*> TextureManager::GetTexture( UUID textureUUID ) const
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() )
			return &existing->second;
		if ( auto existing = m_DefaultTextures.find( textureUUID ); existing != m_DefaultTextures.end() )
			return &existing->second;

		return {};
	}

	/// @brief Fetch a loaded project texture! If not found return a default texture
	/// @param textureUUID UUID of the texture to fetch
	/// @return a mutable pointer to the texture object
	Texture* TextureManager::GetTextureOrDefault( UUID textureUUID )
	{
		return const_cast<Texture*>( static_cast<const TextureManager*>( this )->GetTextureOrDefault( textureUUID ) );
	}

	/// @brief Fetch a loaded project texture! If not found return a default texture
	/// @param textureUUID UUID of the texture to fetch
	/// @return an immutable pointer to the texture object
	const Texture* TextureManager::GetTextureOrDefault( UUID textureUUID ) const
	{
		if ( auto existing = m_Textures.find( textureUUID ); existing != m_Textures.end() )
			return &existing->second;
		if ( auto existing = m_DefaultTextures.find( textureUUID ); existing != m_DefaultTextures.end() )
			return &existing->second;

		return GetDefaultTextureByName( "default_tex" );
	}

	/// @brief Fetch a default texture by the texture name
	/// @param name Name of the default texture
	/// @return a mutable pointer to the texture object
	Texture* TextureManager::GetDefaultTextureByName( const std::string& name )
	{
		return const_cast<Texture*>( static_cast<const TextureManager*>( this )->GetDefaultTextureByName( name ) );
	}

	/// @brief Fetch a default texture by the texture name
	/// @param name Name of the default texture
	/// @return an immutable pointer to the texture object
	const Texture* TextureManager::GetDefaultTextureByName( const std::string& name ) const
	{
		for ( const auto& defaultTexture : m_DefaultTextures )
		{
			if ( defaultTexture.second.GetName() == name )
				return &defaultTexture.second;
		}

		MIKU_ASSERT( false, "The requested default texture by name {} is not loaded!", name );
	}

	/// @brief Fetch a default texture by its filepath
	/// @param path filepath of the texture
	/// @return an optional immutable pointer to the texture object
	std::optional<const Texture*> TextureManager::GetTextureByFilePath( const std::string& path ) const
	{
		for ( const auto [ uuid, tex ] : m_Textures )
			if ( tex.GetPath() == path )
				return { GetTextureOrDefault( uuid ) };

		return std::nullopt;
	}

	/// @brief Fetch a cubemap by its UUID
	/// @param cubemapUUID UUID of the cubemap to fetch
	/// @return an optional mutable pointer to the texture object
	std::optional<Cubemap*> TextureManager::GetCubemap( UUID cubemapUUID )
	{
		return const_cast<Cubemap*>( static_cast<TextureManager*>( this )->GetCubemap( cubemapUUID ).value() );
	}

	/// @brief Fetch a cubemap by its UUID
	/// @param cubemapUUID UUID of the cubemap to fetch
	/// @return an optional immutable pointer to the texture object
	std::optional<const Cubemap*> TextureManager::GetCubemap( UUID cubemapUUID ) const
	{
		if ( auto existing = m_Cubemaps.find( cubemapUUID ); existing != m_Cubemaps.end() )
			return &existing->second;
		return {};
	}

	/// @brief Fetch a texture by its filename
	/// @param filename name of the texture file
	/// @return an immutable pointer to the texture object
	std::optional<const Texture*> TextureManager::GetTextureByName( const std::string& filename ) const
	{
		for ( const auto& [ uuid, textureContainer ] : m_Textures )
		{
			if ( textureContainer.GetName() == filename )
				return &m_Textures.at( uuid );
		}

		return {};
	}

	/// @brief Check whether or not a texture is already loaded; doens't check in deafult textures only project textures
	/// @return bool representing whether or not the texture is loaded
	bool TextureManager::TextureExists( const UUID& uuid ) const
	{
		auto exists = m_Textures.find( uuid );
		return exists != m_Textures.end();
	}

	/// @brief Get all the loaded project textures
	/// @returns A immutable reference to all the textures in UUID, Texture pairs
	const std::unordered_map<UUID, Texture>& TextureManager::GetAllLoadedTextures() const
	{
		return m_Textures;
	}

	/// @brief Get all the loaded default textures
	/// @returns A immutable reference to all the textures in UUID, Texture pairs
	const std::unordered_map<UUID, Texture>& TextureManager::GetAllDefaultTextures() const
	{
		return m_DefaultTextures;
	}

	/// @brief Return default .meta file template
	YAML::Node TextureManager::GetTextureProperties( std::optional<Texture*> texture )
	{
		YAML::Node properties;

		// return the default texture meta file properties
		if ( !texture.has_value() )
		{
			properties[ "wrap" ] = "clamp";
			return properties;
		}

		// return the properties of the texture provided
		switch ( texture.value()->GetWrapMode() )
		{
		case TextureWrapMode::REPEAT:
			properties[ "wrap" ] = "repeat";
			break;
		case TextureWrapMode::CLAMP:
			properties[ "wrap" ] = "clamp";
			break;
		default:
			properties[ "wrap" ] = "clamp";
		}

		return properties;
	}
}
