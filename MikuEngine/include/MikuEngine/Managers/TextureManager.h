#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "AssetManagerBase.h"
#include "Core.h"
#include "Rendering/Cubemap.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	struct TextureIndexEntry
	{
		UUID uuid;
		std::filesystem::path path;
	};

	struct TextureContainer
	{
		TextureIndexEntry index;
		Texture texture;

		std::string GetName() const { return index.path.stem().string(); }
		void SetName( const std::string& newName ) {}
	};

	struct CubemapContainer
	{
		UUID uuid;
		Cubemap cubemap;
	};

	class MIKU_API TextureManager : public AssetManagerBase
	{
	public:
		TextureManager();
		~TextureManager();

		void LoadTexture( const std::string& name, const std::filesystem::path& filepath );

		void LoadAllTextures();
		void LoadAllDefaultTextures();

		void PrepareTextureIndex();
		const std::unordered_map<UUID, TextureContainer>& GetAllLoadedTextures() const;
		const std::unordered_map<UUID, TextureContainer>& GetAllDefaultTextures() const;

		TextureContainer* GetTextureOrDefault( UUID textureUUID );
		const TextureContainer* GetTextureOrDefault( UUID textureUUID ) const;

		std::optional<TextureContainer*> GetTexture( UUID textureUUID );
		std::optional<const TextureContainer*> GetTexture( UUID textureUUID ) const;

		std::optional<CubemapContainer*> GetCubemap( UUID cubemapUUID );
		std::optional<const CubemapContainer*> GetCubemap( UUID cubemapUUID ) const;

		std::optional<const TextureContainer*> GetTextureByName( const std::string& name ) const;

		const TextureContainer* GetDefaultTextureByName( const std::string& name ) const;
		TextureContainer* GetDefaultTextureByName( const std::string& name );

		std::optional<const TextureContainer*> GetTextureByFilePath( const std::string& path ) const;

		bool TextureExists( const UUID& uuid ) const;

		const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid );

		void RenameAssetCleanup( const UUID& uuid, const std::string& newName );
		void DeleteAssetCleanup( const UUID& uuid );

		static YAML::Node GetTextureProperties( std::optional<Texture*> texture );

	private:
		const std::unordered_map<UUID, TextureIndexEntry>& GetTextureIndex() const;

	private:
		bool TextureAlreadyPresent( UUID textureID ) const;

	private:
		std::unordered_map<UUID, TextureIndexEntry> m_TextureIndex;
		std::unordered_map<UUID, TextureIndexEntry> m_DefaultTextureIndex;

		std::unordered_map<UUID, TextureContainer> m_DefaultTextures;
		std::unordered_map<UUID, TextureContainer> m_Textures;

		std::unordered_map<UUID, CubemapContainer> m_Cubemaps;
	};
}
