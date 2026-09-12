#pragma once

#include <filesystem>

#include "yaml-cpp/yaml.h" // IWYU pragma: keep

#include "Core.h"
#include "Data/DefaultModelType.h"
#include "Managers/IAssetManagerBase.h"
#include "Rendering/Model.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API ModelManager : public IAssetManagerBase
	{
	public:
		void Init();
		void InitFrame() override;

		void LoadModel( const std::filesystem::path& filepath, UUID uuid = {} );
		void UnloadModel( const std::filesystem::path& filepath );

		Model* GetModel( UUID modelUUID );
		const Model* GetModel( UUID modelUUID ) const;
		Model* GetModelByName( const std::string& name );
		Model* GetModelByFilePath( const std::filesystem::path& path );

		Model* GetDefaultModel( DefaultModelType type );

		const std::unordered_map<UUID, Model> GetAllModels() const { return m_Models; }
		const std::unordered_map<UUID, Model> GetAllDefaultModels() const { return m_DefaultModels; }

		const std::filesystem::path& GetFilePathByUUID( const UUID& uuid ) override;
		bool ModelExists( const UUID& uuid ) const;

		static YAML::Node GetModelProperties( Model* model );

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
		void LoadAllModels();
		void LoadAllDefaultModels();

		void RenameAssetCleanup( const UUID& uuid, const std::string& newName );
		void DeleteAssetCleanup( const UUID& uuid );

	private:
		std::unordered_map<UUID, Model> m_Models;
		std::unordered_map<UUID, Model> m_DefaultModels;

		std::vector<UUID> m_DeleteQueue{};
		std::vector<std::pair<UUID, std::string>> m_RenameQueue{};
	};
}
