#include "Managers/ModelManager.h"

#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void ModelManager::LoadModel( const std::string& name, const std::filesystem::path& filepath )
	{
		Model newModel;
		newModel.LoadFromFile( filepath );

		m_Models[ UUID() ] = {
		    { name, filepath },
			newModel
		 };
	}

	void ModelManager::PrepareModelIndex()
	{
		// Index Default Models
		if ( std::filesystem::exists( std::filesystem::path( RESOURCE_DIR ) / "models" ) )
		{
			for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/models/" ) )
			{
				if ( file.path().extension() == ".meta" ) continue;

				if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

				UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

				m_DefaultModelIndex[ uuid ] = { file.path().stem().string(), file.path().string() };
			}
		}
		else
			MIKU_CORE_WARN( "Default Model Directory not found! Skipping" );

		// Index Project Models
		if ( std::filesystem::exists( std::filesystem::path( PROJECT_DIR ) / "models" ) )
		{
			for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/models/" ) )
			{
				if ( file.path().extension() == ".meta" ) continue;

				if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

				UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
				m_ModelIndex[ uuid ] = { file.path().stem().string(), file.path().string(), uuid };
			}
		}
		else
			MIKU_CORE_WARN( "Project Model Directory not found! Skipping" );
	}

	void ModelManager::LoadAllModels()
	{
		PrepareModelIndex();

		// Load Project Models
		for ( const auto& [ uuid, index ] : m_ModelIndex )
		{
			Model newModel;
			newModel.LoadFromFile( index.path );

			m_Models[ uuid ] = {
			    index,
			    newModel,
			};
		}

		// Load Default Models
		for ( const auto& [ type, index ] : m_DefaultModelIndex )
		{
			Model newModel;
			newModel.LoadFromFile( index.path );

			m_DefaultModels[ type ] = { index, newModel };
		}
	}

	const ModelContainer& ModelManager::GetModel( UUID modelUUID ) const
	{
		if ( modelUUID == UUID( 0 ) ) return GetDefaultModel( DefaultModelType::CUBE );

		auto count = m_Models.size();
		auto existing = m_Models.find( modelUUID );
		auto exisitngDefault = m_DefaultModels.find( modelUUID );

		if ( existing != m_Models.end() ) return existing->second;
		if ( exisitngDefault != m_DefaultModels.end() ) return exisitngDefault->second;

		MIKU_ASSERT( false, "Model not loaded!" );
	}

	const ModelContainer& ModelManager::GetModelByName( const std::string& name ) const
	{
		for ( const auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.Name == name ) return container;
		}

		MIKU_ASSERT( false, "Requested Model is not loaded!" );
	}

	const ModelContainer& ModelManager::GetModelByFilePath( const std::filesystem::path& path ) const
	{
		for ( const auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.path == path ) return container;
		}

		MIKU_ASSERT( false, "Requested Model is not loaded!" );
	}

	const std::unordered_map<UUID, ModelContainer> ModelManager::GetAllLoadedModels() const
	{
		return m_Models;
	}

	const ModelContainer& ModelManager::GetDefaultModel( DefaultModelType type ) const
	{
		return m_DefaultModels.begin()->second;
	}

	bool ModelManager::ModelExists( const UUID& uuid ) const
	{
		auto existingModel = m_Models.find( uuid );
		return existingModel != m_Models.end();
	}
}
