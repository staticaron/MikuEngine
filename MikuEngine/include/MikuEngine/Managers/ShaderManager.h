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
		std::string name;
		std::string path;
	};

	struct ShaderContainer
	{
		ShaderIndexEntry shaderDetails;
		Shader shader;
	};

	class MIKU_API ShaderManager
	{
	public:
		ShaderManager();

		void LoadShader( const std::string& name, const std::string& filepath );
		void LoadDefaultShaders();
		void LoadAllShaders();

		const std::unordered_map<UUID, ShaderIndexEntry>& GetShaderIndex() const;

		void Refresh();

		const ShaderContainer& GetShader( UUID shaderUUID ) const;
		const ShaderContainer& GetShaderByName( const std::string& name ) const;
		const ShaderContainer& GetShaderByFilePath( const std::filesystem::path& path ) const;
		bool ShaderExists( const UUID& uuid ) const;

		const std::unordered_map<UUID, ShaderContainer>& GetAllLoadedShaders() const;

		std::string GetShaderName( UUID shaderUUID ) const;
		const ShaderContainer& GetDefaultShader() const;

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
