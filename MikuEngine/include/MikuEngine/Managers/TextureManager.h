#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "Rendering/Texture.h"

namespace MikuEngine
{
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		void LoadTexture( std::string_view identifier, const std::string& filepath );
		void LoadAllTextures();

		const Texture& GetTexture( std::string_view identifier ) const
		{
			auto existing = m_Textures.find( identifier );

			return existing->second;
		}

	private:
		bool TextureAlreadyPresent( std::string_view identifier );

	private:
		std::unordered_map<std::string_view, std::string> m_TexturesToLoad = {
		    { "miku", RESOURCE_DIR "textures/base.png" },
		};

		std::unordered_map<std::string_view, Texture> m_Textures;
	};
}
