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

		MIKU_CORE_INFO( "All {} Shaders Loaded!", m_Shaders.size() );
	}

	void ShaderManager::Refresh()
	{
		RefreshShaderIndex();
		RefreshShaders();
	}

	void ShaderManager::RefreshShaderIndex()
	{
		if ( !std::filesystem::exists( PROJECT_DIR "/shaders/" ) ) return;

		unsigned int refreshCount = 0;

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );

			const auto& existingIndex = m_ShaderIndex.find( uuid );

			if ( existingIndex != m_ShaderIndex.end() ) continue;

			m_ShaderIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };

			refreshCount++;
		}

		MIKU_CORE_INFO( "Material Index Refresh Completed with count : {}", refreshCount );
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

		MIKU_CORE_INFO( "Material Refresh Completed with count : {}", refreshCount );
	}

	void ShaderManager::PrepareShaderIndex()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "/shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;

			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_DefaultShaderIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}

		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "/shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;

			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_ShaderIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}

		MIKU_CORE_INFO( "Shader Indexing Complete!", m_ShaderIndex.size() );
	}

	const std::unordered_map<UUID, ShaderIndexEntry>& ShaderManager::GetShaderIndex() const
	{
		return m_ShaderIndex;
	}

	const ShaderContainer& ShaderManager::GetShader( UUID shaderUUID ) const
	{
		if ( shaderUUID == UUID( 0 ) ) return GetDefaultShader();

		auto count = m_Shaders.size();
		auto existing = m_Shaders.find( shaderUUID );

		MIKU_ASSERT( existing != m_Shaders.end(), "Shader not loaded!" );

		return existing->second;
	}

	const ShaderContainer& ShaderManager::GetShaderByName( const std::string& name ) const
	{
		for ( const auto& [ uuid, textureIndexEntry ] : m_ShaderIndex )
		{
			if ( textureIndexEntry.name == name ) return m_Shaders.at( uuid );
		}

		MIKU_ASSERT( false, "Requested Shader is not loaded!" );
	}

	const ShaderContainer& ShaderManager::GetShaderByFilePath( const std::filesystem::path& path ) const
	{
		for ( const auto& [ uuid, textureIndexEntry ] : m_ShaderIndex )
		{
			if ( textureIndexEntry.path == path ) return m_Shaders.at( uuid );
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
		auto existing = m_ShaderIndex.find( textureUUID );
		return existing->second.name;
	}

	const ShaderContainer& ShaderManager::GetDefaultShader() const
	{
		return m_DefaultShaders.begin()->second;
	}
}
