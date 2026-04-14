#pragma once

#include <filesystem>
#include <optional>
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

		void CreateFromShader( const UUID& shader );
		void Refresh();

		void LoadFromFile( const std::filesystem::path& materialPath );
		void SaveToFile( const std::filesystem::path& filePath );

		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& path );

		void RenderInspectorImGui() override;
		void DeleteAsset() override {}

		void Bind();
		void UnBind();

		const UUID& GetUUID() { return m_UUID; }
		const std::filesystem::path& GetPath() const { return m_MaterialPath; }
		std::string GetName() const { return m_MaterialPath.stem().string(); }

		std::optional<Shader*> GetShader();
		void SetShader( const UUID& uuid );

	private:
		void RegisterUniform( std::string, ShaderUniform );

	private:
		UUID m_UUID;

		std::optional<UUID> m_Shader;

		std::filesystem::path m_MaterialPath;

		std::vector<std::string> m_UniformOrder;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
		std::unordered_map<std::string, glm::mat4> m_Mat4s;
	};
}
