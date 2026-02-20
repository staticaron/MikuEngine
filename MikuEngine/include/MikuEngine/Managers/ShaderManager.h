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

		void PrepareShaderIndex();
		const std::unordered_map<UUID, ShaderIndexEntry>& GetShaderIndex() const;

		const ShaderContainer& GetShader( UUID shaderUUID ) const;
		const ShaderContainer& GetShaderByName( const std::string& name ) const;

		const std::unordered_map<UUID, ShaderContainer>& GetAllLoadedShaders() const;

		std::string GetShaderName( UUID shaderUUID ) const;
		const ShaderContainer& GetDefaultShader() const;

	private:
		std::unordered_map<UUID, ShaderIndexEntry> m_ShaderIndex;
		std::unordered_map<UUID, ShaderContainer> m_Shaders;
		std::unordered_map<std::string, ShaderContainer> m_DefaultShaders;
	};
}
