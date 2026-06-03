#include "Managers/ModelManager.h"

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void ModelManager::LoadModel( const std::string& name, const std::filesystem::path& filepath )
	{
		UUID uuid;
		Model newModel( uuid );
		newModel.LoadFromFile( filepath );

		m_Models.insert( {
		    uuid, { { uuid, filepath }, newModel }
		   } );
	}

	void ModelManager::PrepareModelIndex()
	{
		// Index Default Models
		if ( std::filesystem::exists( std::filesystem::path( RESOURCE_DIR ) / "models" ) )
		{
			for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/models/" ) )
			{
				if ( file.path().extension() == ".meta" ) continue;
				if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

				if ( file.is_directory() ) continue;

				UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

				m_DefaultModelIndex[ uuid ] = { uuid, file.path().string() };
			}
		}
		else
			MIKU_CORE_WARN( "Default Model Directory not found! Skipping" );

		// Index Project Models
		const auto& dataContainer = Application::GetDataContainer();

		if ( !std::filesystem::exists( dataContainer.GetProjectAssetPath( "/models" ) ) )
		{
			for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/models/" ) ) )
			{
				if ( file.path().extension() == ".meta" ) continue;

				if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

				UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
				m_ModelIndex[ uuid ] = { uuid, file.path().string() };
			}
		}
		else
			MIKU_CORE_WARN( "Project Model Directory not found! Skipping" );
	}

	void ModelManager::LoadAllModels()
	{
		PrepareModelIndex();

		LoadDefaultModels();

		for ( const auto& [ uuid, index ] : m_ModelIndex )
		{
			Model newModel( uuid );
			newModel.LoadFromFile( index.path );

			m_Models.insert( {
			    uuid, { index, newModel }
			      } );
		}
	}

	void ModelManager::LoadDefaultModels()
	{
		for ( const auto& [ uuid, index ] : m_DefaultModelIndex )
		{
			Model newModel( uuid );
			newModel.LoadFromFile( index.path );

			m_DefaultModels[ uuid ] = { index, newModel };
		}
	}

	std::optional<ModelContainer*> ModelManager::GetModel( UUID modelUUID )
	{
		if ( modelUUID == UUID( 0 ) ) return GetDefaultModel( DefaultModelType::CUBE );

		auto existing = m_Models.find( modelUUID );
		if ( existing != m_Models.end() ) return &existing->second;

		auto exisitngDefault = m_DefaultModels.find( modelUUID );
		if ( exisitngDefault != m_DefaultModels.end() ) return &exisitngDefault->second;

		return {};
	}

	std::optional<ModelContainer*> ModelManager::GetModelByName( const std::string& name )
	{
		for ( auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.GetName() == name ) return &container;
		}

		MIKU_ASSERT( false, "Requested Model is not loaded!" );
	}

	std::optional<ModelContainer*> ModelManager::GetModelByFilePath( const std::filesystem::path& path )
	{
		for ( auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.path == path ) return &container;
		}

		MIKU_ASSERT( false, "Requested Model is not loaded!" );
	}

	std::optional<ModelContainer*> ModelManager::GetDefaultModel( DefaultModelType type )
	{
		return &m_DefaultModels.begin()->second;
	}

	const std::filesystem::path& ModelManager::GetFilePathFromUUID( const UUID& uuid )
	{
		for ( auto& model : m_Models )
		{
			if ( model.first == uuid ) return model.second.index.path;
		}

		MIKU_ASSERT( false, "This Model is not loaded!" );
	}

	bool ModelManager::ModelExists( const UUID& uuid ) const
	{
		auto existingModel = m_Models.find( uuid );
		return existingModel != m_Models.end();
	}

	void ModelManager::RenameAssetCleanup( const UUID& uuid, const std::string& newName )
	{
		if ( auto existing = m_Models.find( uuid ); existing != m_Models.end() )
		{
			existing->second.SetName( newName );
		}
	}

	YAML::Node ModelManager::GetModelProperties( Model* model )
	{
		return {};
	}

	void ModelManager::DeleteAssetCleanup( const UUID& uuid )
	{
		int count = 0;

		if ( auto existing = m_Models.find( uuid ); existing != m_Models.end() )
		{
			m_Models.erase( existing );
			count++;
		}

		if ( count > 0 ) MIKU_CORE_DEBUG( "Model Cleanup Successful! {} Models Deleted!", count );
	}
}
