#pragma once

#include <filesystem>
#include <optional>
#include <string>

#include <glm/glm.hpp>

#include "Core.h"
#include "IAsset.h"
#include "Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	class Asset;
}

namespace MikuEngine
{
	enum class MaterialBlendMode
	{
		TRANSPARENT,
		OPAQUE
	};

	struct MIKU_API RenderOrder
	{
		MaterialBlendMode mode{ MaterialBlendMode::OPAQUE };
		unsigned int order{ 0 };
	};

	class MIKU_API Material : public IAsset
	{
	public:
		Material() : IAsset( AssetType::MATERIAL ) {}
		Material( UUID uuid, const std::filesystem::path& materialPath );

		void CreateFromShader( const UUID& shader );
		void Load( const std::filesystem::path& materialPath );
		void SaveToFile( const std::filesystem::path& filePath );

		Shader* GetShader();
		void SetShader( const UUID& uuid );

		void Refresh();

		void Bind();
		void UnBind();

		const UUID& GetUUID() const override { return m_UUID; }
		std::string GetName() const override { return m_FilePath.stem().string(); }
		const std::filesystem::path& GetPath() const override { return m_FilePath; }
		void SetPath( const std::filesystem::path& path ) override { m_FilePath = path; }

		const std::unordered_map<std::string, UUID>& GetTextures() const { return m_Textures; }
		const std::unordered_map<std::string, UUID>& GetCubemaps() const { return m_Cubemaps; }
		const std::unordered_map<std::string, float>& GetFloats() const { return m_Floats; }
		const std::unordered_map<std::string, glm::vec2>& GetVec2s() const { return m_Vec2s; }

		RenderOrder& GetRenderOrder() { return m_RenderOrder; }
		const RenderOrder& GetRenderOrder() const { return m_RenderOrder; }
		std::string GetBlendModeString() const;

		void SetRenderOrderMode( MaterialBlendMode mode ) { m_RenderOrder.mode = mode; }

		void SetTexture( const std::string& identifier, UUID uuid ) { m_Textures[ identifier ] = uuid; }
		void SetCubemap( const std::string& identifier, UUID uuid ) { m_Cubemaps[ identifier ] = uuid; }
		void SetFloat( const std::string& identifier, float value ) { m_Floats[ identifier ] = value; }
		void SetVec2( const std::string& identifier, glm::vec2 value ) { m_Vec2s[ identifier ] = value; }

	private:
		void RefreshUniforms();
		void RegisterUniform( std::string, ShaderUniform );

	private:
		UUID m_UUID;
		std::filesystem::path m_FilePath{};

		std::optional<UUID> m_Shader;

		std::vector<std::string> m_UniformOrder;

		RenderOrder m_RenderOrder;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, UUID> m_Cubemaps;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, glm::vec2> m_Vec2s;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
		std::unordered_map<std::string, glm::mat4> m_Mat4s;
	};
}
