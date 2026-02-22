#pragma once

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

		void CreateFromShader( const Shader& shader );

		void LoadFromFile( const std::string& materialPath );
		void SaveToFile( const char* filePath ) const;

		void Bind();
		void UnBind();

		void RenderInspectorImGui() override;

	private:
		Shader m_Shader;
		UUID m_ShaderID;

		std::unordered_map<std::string, ShaderUniform> m_Uniforms;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, std::string> m_Strings;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
	};
}
