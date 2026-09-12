#include "Managers/ModelManager.h"

#include "Application.h"
#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	void ModelManager::Init()
	{
		LoadAllModels();
		LoadAllDefaultModels();
	}

	void ModelManager::LoadModel( const std::filesystem::path& filepath )
	{
		UUID uuid;
		Model newModel( filepath, uuid );
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
				if ( file.path().extension() == ".meta" )
					continue;
				if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
					MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

				if ( file.is_directory() )
					continue;

				UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

				m_DefaultModelIndex[ uuid ] = { uuid, file.path().string() };
			}
		}
		else
			MIKU_CORE_WARN( "Default Model Directory not found! Skipping" );

		// Index Project Models
		const auto& dataContainer = Application::GetDataContainer();

		if ( std::filesystem::exists( dataContainer.GetProjectAssetPath( "/models" ) ) )
		{
			for ( auto& file : std::filesystem::recursive_directory_iterator( dataContainer.GetProjectAssetPath( "/models/" ) ) )
			{
				if ( file.path().extension() == ".meta" )
					continue;

				if ( !MetaFileManager::MetaFileExists( file.path().string() ) )
					MetaFileManager::GenerateMetaFile( file.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

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

		for ( const auto& [ uuid, index ] : m_ModelIndex )
		{
			Model newModel( index.path, uuid );
			newModel.LoadFromFile( index.path );

			m_Models.insert( {
			    uuid, { index, newModel }
			      } );
		}

		MIKU_CORE_INFO( "Loading all models! {} models loaded", m_ModelIndex.size() );
	}

	void ModelManager::LoadAllDefaultModels()
	{
		for ( const auto& [ uuid, index ] : m_DefaultModelIndex )
		{
			Model newModel( index.path, uuid );
			newModel.LoadFromFile( index.path );

			m_DefaultModels[ uuid ] = { index, newModel };
		}

		MIKU_CORE_INFO( "Loading all default models! {} default models loaded", m_DefaultModelIndex.size() );
	}

	std::optional<ModelContainer*> ModelManager::GetModel( UUID modelUUID )
	{
		if ( modelUUID == UUID( 0 ) )
			return GetDefaultModel( DefaultModelType::CUBE );

		auto existing = m_Models.find( modelUUID );
		if ( existing != m_Models.end() )
			return &existing->second;

		auto exisitngDefault = m_DefaultModels.find( modelUUID );
		if ( exisitngDefault != m_DefaultModels.end() )
			return &exisitngDefault->second;

		return {};
	}

	std::optional<ModelContainer*> ModelManager::GetModelByName( const std::string& name )
	{
		for ( auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.GetName() == name )
				return &container;
		}

		MIKU_CORE_ERROR( "Requested Model is not loaded!" );
		return std::nullopt;
	}

	std::optional<ModelContainer*> ModelManager::GetModelByFilePath( const std::filesystem::path& path )
	{
		for ( auto& [ uuid, container ] : m_Models )
		{
			if ( container.index.path == path )
				return &container;
		}

		MIKU_CORE_ERROR( "Requested Model is not loaded!" );
		return std::nullopt;
	}

	std::optional<ModelContainer*> ModelManager::GetDefaultModel( DefaultModelType type )
	{
		return &m_DefaultModels.begin()->second;
	}

	const std::filesystem::path& ModelManager::GetFilePathByUUID( const UUID& uuid )
	{
		for ( auto& model : m_Models )
		{
			if ( model.first == uuid )
				return model.second.index.path;
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

		if ( count > 0 )
			MIKU_CORE_DEBUG( "Model Cleanup Successful! {} Models Deleted!", count );
	}

	void ModelManager::InitFrame()
	{
		PerformDeletions();
		PerformRenames();
	};

	/// Add model to the delete queue
	///
	/// @param uuid UUID of the model object
	///
	void ModelManager::AddToDeleteQueue( const UUID& uuid )
	{
		m_DeleteQueue.push_back( uuid );
	};

	/// Add model to the delete queue
	///
	/// @param filepath path of the model asset file
	///
	void ModelManager::AddToDeleteQueue( const std::filesystem::path& filepath )
	{
		if ( auto model = GetModelByFilePath( filepath ); model.has_value() )
			m_DeleteQueue.push_back( model.value()->index.uuid );
	};

	/// Add model to the rename queue
	///
	/// @param uuid UUID of the model object
	/// @param newName new name of the model
	///
	void ModelManager::AddToRenameQueue( const UUID& uuid, const std::string& newName )
	{
		m_RenameQueue.push_back( { uuid, newName } );
	};

	/// Add model to the rename queue
	///
	/// @param filepath path of the model asset file
	/// @param newName new name of the model asset file
	///
	void ModelManager::AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName )
	{
		if ( auto model = GetModelByFilePath( filepath ); model.has_value() )
			m_RenameQueue.push_back( { model.value()->index.uuid, newName } );
	};

	/// Go through all the UUIDs in Delete queue and perform Delete on them
	void ModelManager::PerformDeletions()
	{
		for ( auto modelUUID : m_DeleteQueue )
			DeleteAsset( modelUUID );

		m_DeleteQueue.clear();
	};

	/// Go through all the UUIDs in rename queue and perform Rename on them
	void ModelManager::PerformRenames()
	{
		for ( auto [ modelUUID, newName ] : m_RenameQueue )
			RenameAsset( modelUUID, newName );

		m_RenameQueue.clear();
	};

	/// Delete a model object
	///
	/// @param uuid UUID of the model object to delete
	///
	void ModelManager::DeleteAsset( const UUID& uuid )
	{
		auto modelToDelete = m_Models.find( uuid );

		if ( modelToDelete == m_Models.end() )
			return;

		auto filepath = modelToDelete->second.index.path;

		// Remove from model db
		m_Models.erase( modelToDelete );

		// Delete the Asset Files
		if ( std::filesystem::exists( filepath ) )
			std::filesystem::remove( filepath );

		if ( std::filesystem::exists( filepath.string() + ".meta" ) )
			std::filesystem::remove( filepath.string() + ".meta" );
	};

	/// Rename a model object
	///
	/// @param uuid UUID of the model object to udpate
	/// @param newName new name of the model object
	///
	void ModelManager::RenameAsset( const UUID& uuid, const std::string& newName )
	{
		auto modelToRename = m_Models.find( uuid );

		if ( modelToRename == m_Models.end() )
			return;

		const std::filesystem::path& filePath = modelToRename->second.index.path;
		const std::string fileExtension = filePath.extension();

		// Rename the asset files
		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		// apply the new name to the model object as well
		modelToRename->second.SetName( newName );
	};
}
