#pragma once

#include <filesystem>

#include "Core.h"
#include "Data/DefaultModelType.h"
#include "Rendering/Model.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API ModelIndexEntry
	{
		std::string Name;
		std::filesystem::path path;
		UUID uuid;
	};

	struct MIKU_API ModelContainer
	{
		ModelIndexEntry index;
		Model model;
	};

	class MIKU_API ModelManager
	{
	public:
		void LoadModel( const std::string& name, const std::filesystem::path& filepath );
		void LoadAllModels();
		void LoadDefaultModels();

		void PrepareModelIndex();
		std::unordered_map<UUID, ModelIndexEntry> GetModelIndex() const { return m_ModelIndex; }

		const ModelContainer& GetModel( UUID modelUUID ) const;
		const ModelContainer& GetModelByName( const std::string& name ) const;
		const ModelContainer& GetModelByFilePath( const std::filesystem::path& path ) const;

		const ModelContainer& GetDefaultModel( DefaultModelType type ) const;

		bool ModelExists( const UUID& uuid ) const;

	private:
		std::unordered_map<UUID, ModelIndexEntry> m_ModelIndex;
		std::unordered_map<UUID, ModelIndexEntry> m_DefaultModelIndex;

		std::unordered_map<UUID, ModelContainer> m_Models;
		std::unordered_map<UUID, ModelContainer> m_DefaultModels;
	};
}
