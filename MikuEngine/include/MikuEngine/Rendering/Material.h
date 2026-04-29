#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include <glm/glm.hpp>

#include "Core.h"
#include "Managers/ShaderManager.h"
#include "Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	class Asset;
}

namespace MikuEngine
{
	class MIKU_API Material : public Asset
	{
	public:
		Material() : Asset( AssetType::MATERIAL ) {}
		Material( UUID uuid, const std::filesystem::path& materialPath );

		void CreateFromShader( const UUID& shader );
		void Refresh();

		void LoadFromFile( const std::filesystem::path& materialPath );
		void SaveToFile( const std::filesystem::path& filePath );

		static void CreateAssetAtPath( const std::string& name, const std::filesystem::path& path );

		void DeleteAsset() override;

		void Bind();
		void UnBind();

		const UUID& GetUUID() { return m_UUID; }

		const std::filesystem::path& GetPath() const override;
		std::string GetName() const override;
		void SetName( const std::string& newName ) override;

		std::optional<ShaderContainer*> GetShader();
		void SetShader( const UUID& uuid );

		const std::unordered_map<std::string, UUID>& GetTextures() const { return m_Textures; }
		const std::unordered_map<std::string, float>& GetFloats() const { return m_Floats; }

	private:
		void RefreshUniforms();
		void RegisterUniform( std::string, ShaderUniform );

	private:
		UUID m_UUID;

		std::optional<UUID> m_Shader;

		std::vector<std::string> m_UniformOrder;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
		std::unordered_map<std::string, glm::mat4> m_Mat4s;
	};
}
