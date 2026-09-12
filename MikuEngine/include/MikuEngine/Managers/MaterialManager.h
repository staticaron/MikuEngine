#pragma once

#include "yaml-cpp/node/node.h"

#include "Core.h"

#include "IAssetManagerBase.h"
#include "Rendering/Material.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API MaterialManager : public IAssetManagerBase
	{
	public:
		void Init();

		void InitFrame() override;

		void LoadMaterial( const std::filesystem::path& path, UUID uuid = {} );
		void UnloadMaterial( const std::filesystem::path& filePath );

		void LoadAllMaterials();

		void RefreshMaterials( bool loadIntoExisting = false );

		Material* GetMaterial( const UUID& uuid );
		Material* GetMaterial( const std::string& filepath );
		// TODO: GetMaterialOrDefault( const UUID& uuid);
		const Material* GetMaterial( const UUID& uuid ) const;

		const std::unordered_map<UUID, Material> GetAllMaterials() const;

		const std::filesystem::path& GetFilePathByUUID( const UUID& uuid ) override;

		bool MaterialExists( const UUID& uuid ) const;

		static YAML::Node GetMaterialProperties( Material* material = nullptr );
		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& path );

		void AddToDeleteQueue( const UUID& uuid ) override;
		void AddToDeleteQueue( const std::filesystem::path& filepath ) override;

		void AddToRenameQueue( const UUID& uuid, const std::string& newName ) override;
		void AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName ) override;

	protected:
		void PerformDeletions() override;
		void PerformRenames() override;

		void DeleteAsset( const UUID& uuid ) override;
		void RenameAsset( const UUID& uuid, const std::string& newName ) override;

	private:
		std::unordered_map<UUID, Material> m_Materials;

		std::vector<UUID> m_DeleteQueue{};
		std::vector<std::pair<UUID, std::string>> m_RenameQueue{};
	};
}
