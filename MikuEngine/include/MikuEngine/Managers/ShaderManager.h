#pragma once

#include <string>
#include <unordered_map>

#include "Core.h"
#include "Rendering/Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	struct ShaderIndexEntry
	{
		UUID uuid;
		std::filesystem::path path;
	};

	struct ShaderContainer
	{
		ShaderIndexEntry index;
		Shader shader;

		std::string GetName() const
		{
			// Comment
			return index.path.stem().string();
		}

		void SetName( const std::string& newName )
		{
			std::filesystem::path newNamePath = index.path.parent_path() / ( newName + ".shader" );
			std::filesystem::rename( index.path, newNamePath );
			index.path = newNamePath;
		}
	};

	class MIKU_API ShaderManager
	{
	public:
		ShaderManager();

		void LoadShader( const std::string& name, const std::string& filepath );
		void LoadAllShaders();
		void LoadDefaultShaders();

		void RenameShader( const UUID& uuid, const std::string& newName );
		void DeleteShader( const UUID& uuid );

		void Refresh();

		const std::unordered_map<UUID, ShaderIndexEntry>& GetShaderIndex() const;
		const std::unordered_map<UUID, ShaderContainer>& GetAllLoadedShaders() const;

		ShaderContainer& GetShader( UUID shaderUUID );
		const ShaderContainer& GetShader( UUID shaderUUID ) const;
		const ShaderContainer& GetDefaultShader() const;

		ShaderContainer& GetShaderByName( const std::string& name );
		ShaderContainer& GetShaderByFilePath( const std::filesystem::path& path );
		std::string GetShaderName( UUID shaderUUID ) const;

		bool ShaderExists( const UUID& uuid ) const;

	private:
		void PrepareShaderIndex();

		void RefreshShaderIndex();
		void RefreshShaders();

	private:
		std::unordered_map<UUID, ShaderIndexEntry> m_ShaderIndex;
		std::unordered_map<UUID, ShaderIndexEntry> m_DefaultShaderIndex;

		std::unordered_map<UUID, ShaderContainer> m_Shaders;
		std::unordered_map<UUID, ShaderContainer> m_DefaultShaders;
	};
}
