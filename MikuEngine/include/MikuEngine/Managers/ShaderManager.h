#pragma once

#include <string>
#include <unordered_map>

#include "Core.h"
#include "IAssetManagerBase.h"
#include "Rendering/Shader.h"
#include "UUID.h"
#include "yaml-cpp/node/node.h"

namespace MikuEngine
{
	constexpr std::string_view DEFAULT_2D_SHADER_LOCATION = RESOURCE_DIR "/shaders/default-2d.shader";

	class MIKU_API ShaderManager : public IAssetManagerBase
	{
	public:
		void Init();

		void LoadShader( const std::filesystem::path& filepath, UUID uuid = {} );
		void LoadDefaultShader( const std::filesystem::path& filepath, UUID uuid = {} );

		void LoadAllShaders( bool loadExisting = false );
		void RefreshShaders();

		Shader* GetShader( UUID shaderUUID );
		const Shader* GetShader( UUID shaderUUID ) const;
		const Shader* GetShaderOrDefault( UUID shaderUUID ) const;
		const Shader* GetDefaultShader() const;
		const Shader* GetDefaultShader( UUID shaderUUID ) const;

		const std::string& GetShaderIncludeCode( const std::string& identifier ) const;

		const std::unordered_map<UUID, Shader>& GetAllLoadedShaders() const { return m_Shaders; }
		const std::unordered_map<UUID, Shader>& GetAllDefaultShaders() const { return m_DefaultShaders; }

		Shader* GetShaderByName( const std::string& name );
		Shader* GetShaderByFilePath( const std::filesystem::path& path );
		std::string GetShaderName( UUID shaderUUID ) const;

		void InitFrame() override;

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
		void LoadAllDefaultShaders( bool loadExisting );
		void LoadShaderIncludes();

	private:
		std::unordered_map<UUID, Shader> m_Shaders;
		std::unordered_map<UUID, Shader> m_DefaultShaders;

		std::unordered_map<std::string, std::string> m_ShaderIncludes;

		std::vector<UUID> m_DeleteQueue{};
		std::vector<std::pair<UUID, std::string>> m_RenameQueue{};
	};
}
