#pragma once

#include <filesystem>
#include <string>

#include <glm/glm.hpp>

#include "Core.h"
#include "Logger.h"
#include "Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	class Asset;
}

namespace MikuEngine
{
	class MIKU_API Material : Asset
	{
	public:
		Material() : Asset( AssetType::MATERIAL ) {}
		Material( UUID uuid ) : Asset( AssetType::MATERIAL ), m_UUID( uuid ) {}

		void CreateFromShader( const Shader& shader );

		void LoadFromFile( const std::filesystem::path& materialPath );
		void SaveToFile( const std::filesystem::path& filePath ) const;

		const UUID& GetUUID() { return m_UUID; }

		void Bind();
		void UnBind();

		void RenderInspectorImGui() override;

	private:
		void RegisterUniform( std::string, ShaderUniform );

	private:
		UUID m_UUID;

		Shader m_Shader;
		UUID m_ShaderID;

		std::string m_MaterialName;
		std::string m_FilePath;

		std::vector<std::string> m_UniformOrder;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, std::string> m_Strings;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
	};
}
