#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Material
	{
	public:
		void Init( const std::string& materialPath );

		void Serialize( const char* filePath );

		void Bind();
		void UnBind();

	private:
		UUID m_Shader = 0;

		std::unordered_map<std::string, UUID> m_UUIDs;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, std::string> m_Strings;
		std::unordered_map<std::string, glm::vec4> m_Vec4s;
	};
}
