#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "yaml-cpp/node/node.h"

#include "Core.h"
#include "IAssetManagerBase.h"
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

	class MIKU_API TextureManager : public IAssetManagerBase
	{
	public:
		void Init();

		void InitFrame() override;

		void LoadTexture( const std::filesystem::path& filepath, const UUID& uuid = {} );
		void LoadDefaultTexture( const std::filesystem::path& filepath, const UUID& uuid = {} );

		void LoadAllTextures();
		void LoadAllDefaultTextures();

		Texture* GetTextureOrDefault( UUID textureUUID );
		const Texture* GetTextureOrDefault( UUID textureUUID ) const;

		Texture* GetDefaultTextureByName( const std::string& name );
		const Texture* GetDefaultTextureByName( const std::string& name ) const;

		std::optional<const Texture*> GetTextureByName( const std::string& name ) const;

		std::optional<Texture*> GetTexture( UUID textureUUID );
		std::optional<const Texture*> GetTexture( UUID textureUUID ) const;

		std::optional<Cubemap*> GetCubemap( UUID cubemapUUID );
		std::optional<const Cubemap*> GetCubemap( UUID cubemapUUID ) const;

		std::optional<const Texture*> GetTextureByFilePath( const std::string& path ) const;

		void AddToDeleteQueue( const UUID& uuid ) override;
		void AddToDeleteQueue( const std::filesystem::path& filepath ) override;

		void AddToRenameQueue( const UUID& uuid, const std::string& newName ) override;
		void AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName ) override;

		const std::unordered_map<UUID, Texture>& GetAllLoadedTextures() const;
		const std::unordered_map<UUID, Texture>& GetAllDefaultTextures() const;

		bool TextureExists( const UUID& uuid ) const;

		void Destroy();

		static YAML::Node GetTextureProperties( Texture* texture = nullptr );

	protected:
		void PerformDeletions() override;
		void PerformRenames() override;

		void DeleteAsset( const UUID& uuid ) override;
		void RenameAsset( const UUID& uuid, const std::string& newName ) override;

		const std::filesystem::path& GetFilePathByUUID( const UUID& uuid ) override;

	private:
		std::vector<UUID> m_DeleteQueue{};
		std::vector<std::pair<UUID, std::string>> m_RenameQueue{};

		std::unordered_map<UUID, Texture> m_Textures{};
		std::unordered_map<UUID, Texture> m_DefaultTextures{};
		std::unordered_map<UUID, Cubemap> m_Cubemaps{};
	};
}
