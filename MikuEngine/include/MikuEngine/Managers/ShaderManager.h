#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "AssetManagerBase.h"
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

		std::string GetName() const { return index.path.stem().string(); }

		void SetName( const std::string& newName )
		{
			std::filesystem::path newFilePath = index.path.parent_path() / ( newName + ".shader" );
			index.path = newFilePath;
		}
	};

	class MIKU_API ShaderManager : public AssetManagerBase
	{
	public:
		ShaderManager();

		void LoadShader( const std::string& name, const std::string& filepath );
		void LoadAllShaders();

		void Refresh();

		std::optional<ShaderContainer*> GetShader( UUID shaderUUID );
		const ShaderContainer& GetShader( UUID shaderUUID ) const;
		const ShaderContainer& GetDefaultShader() const;

		const std::string& GetShaderIncludeCode( const std::string& identifier ) const;

		const std::unordered_map<UUID, ShaderContainer>& GetAllLoadedShaders() const { return m_Shaders; }
		const std::unordered_map<UUID, ShaderContainer>& GetAllDefaultShaders() const { return m_DefaultShaders; }

		ShaderContainer& GetShaderByName( const std::string& name );
		std::optional<const ShaderContainer*> GetShaderByFilePath( const std::filesystem::path& path );
		std::string GetShaderName( UUID shaderUUID ) const;

		bool ShaderExists( const UUID& uuid ) const;
		void RenameAssetCleanup( const UUID& uuid, const std::string& newName ) override;

	private:
		void PrepareShaderIndex();
		void LoadDefaultShaders();
		void LoadShaderIncludes();

		void RefreshShaderIndex();
		void RefreshShaders();

		const std::unordered_map<UUID, ShaderIndexEntry>& GetShaderIndex() const;

		const std::filesystem::path& GetFilePathFromUUID( const UUID& uuid ) override;
		void DeleteAssetCleanup( const UUID& uuid ) override;

	private:
		std::unordered_map<UUID, ShaderIndexEntry> m_ShaderIndex;
		std::unordered_map<UUID, ShaderIndexEntry> m_DefaultShaderIndex;

		std::unordered_map<UUID, ShaderContainer> m_Shaders;
		std::unordered_map<UUID, ShaderContainer> m_DefaultShaders;

		std::unordered_map<std::string, std::string> m_ShaderIncludes;
	};
}
