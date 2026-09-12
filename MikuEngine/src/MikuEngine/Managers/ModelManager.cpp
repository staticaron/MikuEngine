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

	void ModelManager::UnloadModel( const std::filesystem::path& filepath )
	{
		auto model = m_Models.begin();

		// Find the iterator with same filepath
		while ( model != m_Models.end() )
		{
			if ( model->second.GetPath() == filepath )
				break;

			model++;
		}

		// Model not loaded! Can't unload
		if ( model == m_Models.end() )
		{
			MIKU_CORE_DEBUG( "Tried to Remove Model named {} but no model of that name was found!", filepath.stem().c_str() );
			return;
		}

		// Delete the model
		m_Models.erase( model );
		MetaFileManager::DeleteMetaFile( filepath );

		MIKU_CORE_DEBUG( "[UNLOADED] Model Removed named {}", filepath.stem().c_str() );
	}

	void ModelManager::InitFrame()
	{
		PerformDeletions();
		PerformRenames();
	};

	void ModelManager::LoadModel( const std::filesystem::path& filepath, UUID uuid )
	{
		Model newModel( filepath, uuid );
		m_Models.insert( { uuid, newModel } );
	}

	/// Load all the models by going through the model assets in the model directory
	void ModelManager::LoadAllModels()
	{
		const auto& data = Application::GetDataContainer();

		auto modelDirectory = data.GetProjectAssetPath( "/models" );

		if ( std::filesystem::exists( modelDirectory ) == false )
		{
			MIKU_CORE_WARN( "Project Model Directory not found! Skipping" );
			return;
		}

		for ( auto& item : std::filesystem::recursive_directory_iterator( modelDirectory ) )
		{
			if ( item.path().extension() == ".meta" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

			LoadModel( item.path(), uuid );
		}

		MIKU_CORE_INFO( "Loading all models! {} models loaded", m_Models.size() );
	}

	/// Load all the default models by going through the default model asset directory
	void ModelManager::LoadAllDefaultModels()
	{
		auto defaultModelDirectory = std::filesystem::path( RESOURCE_DIR ) / "models";

		if ( std::filesystem::exists( defaultModelDirectory ) == false )
		{
			MIKU_CORE_WARN( "Default Model Directory not found! Skipping" );
			return;
		}

		for ( auto& item : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/models/" ) )
		{
			if ( item.path().extension() == ".meta" || item.is_directory() )
				continue;

			auto uuid = MetaFileManager::GenerateMetaFileIfNotPresent( item.path().string(), AssetType::MODEL, ModelManager::GetModelProperties( nullptr ) );

			LoadModel( item.path(), uuid );
		}

		MIKU_CORE_INFO( "Loading all default models! {} default models loaded", m_DefaultModels.size() );
	}

	/// Fetch a model by looking for it in project as well as default model list
	///
	/// @param modelUUID UUID of the model to fetch
	/// @return a pointer to the model object, nullptr if the model doesn't exists
	///
	Model* ModelManager::GetModel( UUID modelUUID )
	{
		if ( modelUUID == UUID( 0 ) )
			return GetDefaultModel( DefaultModelType::CUBE );

		auto existing = m_Models.find( modelUUID );
		if ( existing != m_Models.end() )
			return &existing->second;

		auto exisitngDefault = m_DefaultModels.find( modelUUID );
		if ( exisitngDefault != m_DefaultModels.end() )
			return &exisitngDefault->second;

		return nullptr;
	}

	Model* ModelManager::GetModelByName( const std::string& name )
	{
		for ( auto& [ uuid, model ] : m_Models )
		{
			if ( model.GetName() == name )
				return &model;
		}

		MIKU_CORE_ERROR( "Requested Model is not loaded!" );

		return nullptr;
	}

	Model* ModelManager::GetModelByFilePath( const std::filesystem::path& path )
	{
		for ( auto& [ uuid, model ] : m_Models )
		{
			if ( model.GetPath() == path )
				return &model;
		}

		MIKU_CORE_ERROR( "Requested Model is not loaded!" );

		return nullptr;
	}

	Model* ModelManager::GetDefaultModel( DefaultModelType type )
	{
		return &m_DefaultModels.begin()->second;
	}

	const std::filesystem::path& ModelManager::GetFilePathByUUID( const UUID& uuid )
	{
		if ( auto model = m_Models.find( uuid ); model != m_Models.end() )
			return model->second.GetPath();

		MIKU_ASSERT( false, "This Model is not loaded! Can't return filepath" );
	}

	bool ModelManager::ModelExists( const UUID& uuid ) const
	{
		auto existingModel = m_Models.find( uuid );
		return existingModel != m_Models.end();
	}

	YAML::Node ModelManager::GetModelProperties( Model* model )
	{
		return {};
	}

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
		if ( auto model = GetModelByFilePath( filepath ); model != nullptr )
			m_DeleteQueue.push_back( model->GetUUID() );
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
		if ( auto model = GetModelByFilePath( filepath ); model != nullptr )
			m_RenameQueue.push_back( { model->GetUUID(), newName } );
	};

	/// Go through all the UUIDs in Delete queue and perform Delete on them
	void ModelManager::PerformDeletions()
	{
		if ( m_DeleteQueue.size() <= 0 )
			return;

		for ( auto modelUUID : m_DeleteQueue )
			DeleteAsset( modelUUID );

		MIKU_CORE_INFO( "{} Models Deleted", m_DeleteQueue.size() );

		m_DeleteQueue.clear();
	};

	/// Go through all the UUIDs in rename queue and perform Rename on them
	void ModelManager::PerformRenames()
	{
		if ( m_RenameQueue.size() <= 0 )
			return;

		for ( auto [ modelUUID, newName ] : m_RenameQueue )
			RenameAsset( modelUUID, newName );

		MIKU_CORE_INFO( "{} Models Renamed", m_RenameQueue.size() );

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

		auto filepath = modelToDelete->second.GetPath();

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

		const std::filesystem::path& filePath = modelToRename->second.GetPath();
		const std::string fileExtension = filePath.extension();

		// Rename the asset files
		std::filesystem::path newFilePath = filePath.parent_path() / ( newName + fileExtension );
		std::filesystem::path newMetaFilePath = filePath.parent_path() / ( newName + fileExtension + ".meta" );

		std::filesystem::rename( filePath, newFilePath );
		std::filesystem::rename( filePath.string() + ".meta", newMetaFilePath );

		// apply the new name to the model object as well
		modelToRename->second.SetPath( newFilePath );
	};
}
