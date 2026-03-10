#pragma once

#include <filesystem>
#include <string>

#include <glm/glm.hpp>

#include "Core.h"
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
		Material( UUID uuid, const std::filesystem::path& materialPath );

		void CreateFromShader( const Shader& shader );

		void LoadFromFile( const std::filesystem::path& materialPath );
		void SaveToFile( const std::filesystem::path& filePath ) const;

		const UUID& GetUUID() { return m_UUID; }

		void Bind();
		void UnBind();

		const std::filesystem::path& GetPath() const { return m_MaterialPath; }
		std::string GetName() const { return m_MaterialPath.stem().string(); }

		void RenderInspectorImGui() override;

	private:
		void RegisterUniform( std::string, ShaderUniform );

	private:
		UUID m_UUID;

		Shader m_Shader;
		UUID m_ShaderID;

		std::filesystem::path m_MaterialPath;

		std::vector<std::string> m_UniformOrder;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, std::string> m_Strings;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
	};
}
