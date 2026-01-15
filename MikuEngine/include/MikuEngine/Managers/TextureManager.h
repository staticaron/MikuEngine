#pragma once

#include <string>
#include <unordered_map>

#include "Core.h"
#include "UUID.h"

#include "Rendering/Texture.h"

namespace MikuEngine
{
	struct TextureIndexEntry
	{
		UUID uuid;
		std::string name;
		std::string path;
	};

	class MIKU_API TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		void LoadTexture( const std::string& name, const std::string& filepath );
		void LoadAllTextures();

		void PrepareTextureIndex();
		const std::unordered_map<UUID, TextureIndexEntry>& GetTextureIndex() const;

		const Texture& GetTexture( UUID textureUUID ) const;
		const Texture& GetTextureByName( const std::string& name ) const;

		const std::unordered_map<UUID, Texture>& GetAllLoadedTextures() const;

		std::string GetTextureName( UUID textureUUID ) const;

	private:
		bool TextureAlreadyPresent( UUID textureID ) const;

	private:
		std::unordered_map<UUID, TextureIndexEntry> m_TextureIndex;

		std::unordered_map<UUID, Texture> m_Textures;
	};
}
