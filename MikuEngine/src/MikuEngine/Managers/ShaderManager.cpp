#include "Managers/ShaderManager.h"

#include <filesystem>

#include "Logger.h"
#include "Managers/MetaFileManager.h"
#include "Rendering/Shader.h"

namespace MikuEngine
{
	ShaderManager::ShaderManager()
	{
		PrepareShaderIndex();
	}

	void ShaderManager::LoadShader( const std::string& name, const std::string& filepath )
	{
		Shader shaderContainer;
		shaderContainer.LoadFromFile( filepath );

		m_Shaders[ UUID() ] = shaderContainer;
	}

	void ShaderManager::LoadDefaultShaders()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( RESOURCE_DIR "shaders/" ) )
		{
			Shader defaultShader;
			defaultShader.LoadFromFile( file.path().string() );

			m_DefaultShaders[ file.path().stem().string() ] = defaultShader;
		}
	}

	void ShaderManager::LoadAllShaders()
	{
		LoadDefaultShaders();

		for ( auto [ uuid, shaderIndex ] : m_ShaderIndex )
		{
			Shader shaderContainer;
			shaderContainer.LoadFromFile( shaderIndex.path );

			m_Shaders[ uuid ] = shaderContainer;
		}
	}

	void ShaderManager::PrepareShaderIndex()
	{
		for ( auto& file : std::filesystem::recursive_directory_iterator( PROJECT_DIR "shaders/" ) )
		{
			if ( file.path().extension() == ".meta" ) continue;
			if ( !MetaFileManager::MetaFileExists( file.path().string() ) ) MetaFileManager::GenerateMetaFile( file.path().string() );

			UUID uuid = MetaFileManager::GetUUIDFromMetaFile( file.path() );
			m_ShaderIndex[ uuid ] = { uuid, file.path().stem().string(), file.path().string() };
		}

		MIKU_CORE_INFO( "Shader Indexing Complete!" );
	}

	const std::unordered_map<UUID, ShaderIndexEntry>& ShaderManager::GetShaderIndex() const
	{
		return m_ShaderIndex;
	}

	const Shader& ShaderManager::GetShader( UUID shaderUUID ) const
	{
		auto existing = m_Shaders.find( shaderUUID );

		MIKU_ASSERT( existing != m_Shaders.end(), "Shader not loaded!" );

		return existing->second;
	}

	const Shader& ShaderManager::GetShaderByName( const std::string& name ) const
	{
		for ( const auto& [ uuid, textureIndexEntry ] : m_ShaderIndex )
		{
			if ( textureIndexEntry.name == name ) return m_Shaders.at( uuid );
		}

		MIKU_ASSERT( false, "Requested Texture is not loaded!" );
	}

	const std::unordered_map<UUID, Shader>& ShaderManager::GetAllLoadedShaders() const
	{
		return m_Shaders;
	}

	std::string ShaderManager::GetShaderName( UUID textureUUID ) const
	{
		auto existing = m_ShaderIndex.find( textureUUID );
		return existing->second.name;
	}

	const Shader& ShaderManager::GetDefaultShader() const
	{
		return m_DefaultShaders.at( "quad" );
	}

}
