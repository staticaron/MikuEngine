#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Core.h"
#include "IAssetManagerBase.h"
#include "Rendering/Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	constexpr std::string_view DEFAULT_2D_SHADER_LOCATION = RESOURCE_DIR "/shaders/default-2d.shader";

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

	class MIKU_API ShaderManager : public IAssetManagerBase
	{
	public:
		void Init();
		void InitFrame() override;

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

		void AddToDeleteQueue( const UUID& uuid ) override;
		void AddToDeleteQueue( const std::filesystem::path& filepath ) override;

		void AddToRenameQueue( const UUID& uuid, const std::string& newName ) override;
		void AddToRenameQueue( const std::filesystem::path& filepath, const std::string& newName ) override;

		bool ShaderExists( const UUID& uuid ) const;

		static YAML::Node GetShaderProperties( Shader* shader );
		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& folderPath );

	protected:
		void PerformDeletions() override;
		void PerformRenames();

		void DeleteAsset( const UUID& uuid ) override;
		void RenameAsset( const UUID& uuid, const std::string& newName ) override;

		const std::filesystem::path& GetFilePathByUUID( const UUID& uuid ) override;

	private:
		void PrepareShaderIndex();
		void LoadDefaultShaders();
		void LoadShaderIncludes();

		void RefreshShaderIndex();
		void RefreshShaders();

		const std::unordered_map<UUID, ShaderIndexEntry>& GetShaderIndex() const;

	private:
		std::unordered_map<UUID, ShaderIndexEntry> m_ShaderIndex;
		std::unordered_map<UUID, ShaderIndexEntry> m_DefaultShaderIndex;

		std::unordered_map<UUID, ShaderContainer> m_Shaders;
		std::unordered_map<UUID, ShaderContainer> m_DefaultShaders;

		std::unordered_map<std::string, std::string> m_ShaderIncludes;

		//===================
		std::vector<UUID> m_DeleteQueue{};
		std::vector<std::pair<UUID, std::string>> m_RenameQueue{};
	};
}
