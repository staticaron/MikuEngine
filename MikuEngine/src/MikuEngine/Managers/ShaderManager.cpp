#include "Managers/ShaderManager.h"

#include <filesystem>

#include "Logger.h"
#include "Managers/MetaFileManager.h"

namespace MikuEngine
{
	ShaderManager::ShaderManager()
	{
		PrepareShaderIndex();
	}

	void ShaderManager::InitFrame()
	{
		// Perform deletions queued during the previous frame
		PerformDeletions();
	}

	void ShaderManager::LoadShader( const std::string& name, const std::string& filepath )
	{
		UUID newUUID;
		Shader shader( newUUID, filepath );

		m_Shaders[ newUUID ] = {
		    { name, filepath },
			shader
		       };
	}

	void ShaderManager::LoadDefaultShaders()
	{
		for ( auto [ uuid, shaderIndex ] : m_DefaultShaderIndex )
		{
			Shader shader( uuid, shaderIndex.path );
			m_DefaultShaders[ uuid ] = { shaderIndex, shader };
		}
	}

	void ShaderManager::LoadAllShaders()
	{
		LoadDefaultShaders();

		for ( auto [ uuid, shaderIndex ] : m_ShaderIndex )
		{
			Shader shader( uuid, shaderIndex.path );
			m_Shaders[ uuid ] = { shaderIndex, shader };
		}

		MIKU_CORE_DEBUG( "All {} Shaders Loaded!", m_Shaders.size() );
	}

	void ShaderManager::RenameShader( const UUID& uuid, const std::string& newName )
	{
		const auto& existing = m_Shaders.find( uuid );
		MIKU_ASSERT( existing != m_Shaders.end(), "Shader not found! Aborting Rename" );

		existing->second.SetName( newName );

		MIKU_CORE_INFO( "Shader New Path : {}", existing->second.GetName() );
	}

	void ShaderManager::DeleteShader( const UUID& uuid )
	{
		MIKU_CORE_INFO( "Number of Shaders before removal : {}", m_Shaders.size() );

		auto shader = GetShader( uuid );

		if ( shader.has_value() == false ) return;

		if ( std::filesystem::exists( shader.value().get().index.path ) ) std::filesystem::remove( shader.value().get().index.path );

		const auto& existing = m_Shaders.find( uuid );
		m_Shaders.erase( existing );

		MIKU_CORE_INFO( "Number of Shaders after removal : {}", m_Shaders.size() );
	}

	void ShaderManager::Refresh()
	{
		PrepareShaderIndex();
		RefreshShaders();
	}

	void ShaderManager::PerformDeletions()
	{
		unsigned int deleteCount = 0;

		for ( const UUID& uuid : m_DeleteQueue )
		{
			DeleteShader( uuid );
			deleteCount++;
		}

		if ( deleteCount > 0 ) MIKU_CORE_INFO( "{} Shaders Deleted", deleteCount );

		m_DeleteQueue.clear();
	}

	void ShaderManager::RefreshShaders()
	{
		unsigned int refreshCount = 0;

		for ( const auto& [ uuid, index ] : m_ShaderIndex )
		{
			const auto& existing = m_Shaders.find( uuid );

			if ( existing != m_Shaders.end() ) continue;

			Shader shader( uuid, index.path );
			m_Shaders[ uuid ] = { index, shader };

			refreshCount++;
		}

		MIKU_CORE_DEBUG( "Material Refresh Completed with count : {}", refreshCount );
	}

	void ShaderManager::PrepareShaderIndex()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;

			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_DefaultShaderIndex[ uuid ] = { uuid, file.path().string() };
		}

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;

			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_ShaderIndex[ uuid ] = { uuid, file.path().string() };
		}

		MIKU_CORE_DEBUG( "Shader Indexing Complete!", m_ShaderIndex.size() );
	}

	const std::unordered_map<UUID, ShaderIndexEntry>& ShaderManager::GetShaderIndex() const
	{
		return m_ShaderIndex;
	}

	std::optional<std::reference_wrapper<ShaderContainer>> ShaderManager::GetShader( UUID shaderUUID )
	{
		auto existing = m_Shaders.find( shaderUUID );

		if ( existing == m_Shaders.end() ) return std::nullopt;

		return std::ref( existing->second );
	}

	const ShaderContainer& ShaderManager::GetShader( UUID shaderUUID ) const
	{
		if ( shaderUUID == UUID( 0 ) ) return GetDefaultShader();

		auto count = m_Shaders.size();
		auto existing = m_Shaders.find( shaderUUID );

		MIKU_ASSERT( existing != m_Shaders.end(), "Shader not loaded!" );

		return existing->second;
	}

	ShaderContainer& ShaderManager::GetShaderByName( const std::string& name )
	{
		for ( auto& [ uuid, shaderContainer ] : m_Shaders )
		{
			if ( shaderContainer.GetName() == name ) return shaderContainer;
		}

		MIKU_ASSERT( false, "Requested Shader is not loaded!" );
	}

	ShaderContainer& ShaderManager::GetShaderByFilePath( const std::filesystem::path& path )
	{
		for ( const auto& [ uuid, shaderContainer ] : m_Shaders )
		{
			if ( shaderContainer.index.path == path ) return m_Shaders.at( uuid );
		}

		MIKU_ASSERT( false, "Requested Shader is not loaded!" );
	}

	bool ShaderManager::ShaderExists( const UUID& uuid ) const
	{
		auto exists = m_Shaders.find( uuid );
		return exists != m_Shaders.end();
	}

	const std::unordered_map<UUID, ShaderContainer>& ShaderManager::GetAllLoadedShaders() const
	{
		return m_Shaders;
	}

	std::string ShaderManager::GetShaderName( UUID textureUUID ) const
	{
		auto existing = m_Shaders.find( textureUUID );
		return existing->second.GetName();
	}

	const ShaderContainer& ShaderManager::GetDefaultShader() const
	{
		return m_DefaultShaders.begin()->second;
	}
}
