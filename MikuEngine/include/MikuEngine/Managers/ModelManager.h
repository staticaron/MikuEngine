#pragma once

#include <filesystem>
#include <optional>

#include "yaml-cpp/yaml.h"

#include "Core.h"
#include "Data/DefaultModelType.h"
#include "Managers/AssetManagerBase.h"
#include "Rendering/Model.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API ModelIndexEntry
	{
		UUID uuid;
		std::filesystem::path path;

		std::string GetName() const { return path.stem().string(); }
	};

	struct MIKU_API ModelContainer
	{
		ModelIndexEntry index;
		Model model;

		std::string GetName() const { return index.path.stem().string(); }
		void SetName( const std::string& newName )
		{
			std::filesystem::path newFilePath = index.path.parent_path() / ( newName + index.path.extension().string() );
			index.path = newFilePath;
		}
	};

	class MIKU_API ModelManager : public AssetManagerBase
	{
	public:
		void LoadModel( const std::string& name, const std::filesystem::path& filepath );
		void LoadAllModels();
		void LoadDefaultModels();

		void PrepareModelIndex();
		std::unordered_map<UUID, ModelIndexEntry> GetModelIndex() const { return m_ModelIndex; }

		std::optional<ModelContainer*> GetModel( UUID modelUUID );
		std::optional<ModelContainer*> GetDefaultModel( DefaultModelType type );
		std::optional<ModelContainer*> GetModelByName( const std::string& name );
		std::optional<ModelContainer*> GetModelByFilePath( const std::filesystem::path& path );

		const std::unordered_map<UUID, ModelContainer> GetAllLoadedModels() const { return m_Models; }
		const std::unordered_map<UUID, ModelContainer> GetAllDefaultModels() const { return m_DefaultModels; }
		const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid ) override;
		bool ModelExists( const UUID& uuid ) const;

		void RenameAssetCleanup( const UUID& uuid, const std::string& newName ) override;

		static YAML::Node GetModelProperties( Model* model );

	private:
		void DeleteAssetCleanup( const UUID& uuid ) override;

	private:
		std::unordered_map<UUID, ModelIndexEntry> m_ModelIndex;
		std::unordered_map<UUID, ModelIndexEntry> m_DefaultModelIndex;

		std::unordered_map<UUID, ModelContainer> m_Models;
		std::unordered_map<UUID, ModelContainer> m_DefaultModels;
	};
}
