#pragma once

#include <optional>

#include "Core.h"

#include "Rendering/Material.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API MaterialIndex
	{
		UUID uuid;
		std::string name;
		std::string path;
	};

	struct MIKU_API MaterialContainer
	{
		UUID uuid;
	};

	class MIKU_API MaterialManager
	{
	public:
		void LoadAllMaterials();
		void LoadMaterial( Material mat );

		void PrepareMaterialIndex();

		std::optional<Material*> GetMaterial( UUID uuid );
		std::optional<const Material*> GetMaterial( UUID uuid ) const;
		bool MaterialExists( const UUID& uuid ) const;

		std::optional<Material*> GetMaterialByFilePath( const std::string& filepath );

	private:
		std::unordered_map<UUID, MaterialIndex> m_MaterialIndex;
		std::unordered_map<UUID, Material> m_Materials;
	};
}
