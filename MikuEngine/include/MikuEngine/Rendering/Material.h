#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Core.h"
#include "Shader.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Material
	{
	public:
		void CreateFromShader( const Shader& shader );

		void LoadFromFile( const std::string& materialPath );
		void SaveToFile( const char* filePath ) const;

		void Bind();
		void UnBind();

	private:
		Shader m_Shader;
		UUID m_ShaderID;

		std::unordered_map<std::string, UUID> m_Textures;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, std::string> m_Strings;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
	};
}
