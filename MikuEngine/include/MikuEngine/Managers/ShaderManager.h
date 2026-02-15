#pragma once

#include <string>
#include <unordered_map>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class Shader;
}

namespace MikuEngine
{
	struct ShaderIndexEntry
	{
		UUID uuid;
		std::string name;
		std::string path;
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

		const Shader& GetShader( UUID textureUUID ) const;
		const Shader& GetShaderByName( const std::string& name ) const;

		const std::unordered_map<UUID, Shader>& GetAllLoadedShaders() const;

		std::string GetShaderName( UUID textureUUID ) const;
		const Shader& GetDefaultShader() const;

	private:
		std::unordered_map<UUID, ShaderIndexEntry> m_ShaderIndex;
		std::unordered_map<UUID, Shader> m_Shaders;
		std::unordered_map<std::string, Shader> m_DefaultShaders;
	};
}
