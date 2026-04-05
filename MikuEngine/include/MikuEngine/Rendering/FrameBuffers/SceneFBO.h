#pragma once

#include "Core.h"

#include "glm/glm.hpp"

namespace MikuEngine
{
	class MIKU_API SceneFBO
	{
	public:
		void Init();

		void Bind() const;
		void UnBind() const;

		void Destroy();

		void ResizeBufferTexture( glm::vec2 viewPortSize );

		unsigned int GetTextureID() const { return m_ColorTextureID; }

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_ColorTextureID = 0;
		unsigned int m_DepthTextureID = 0;
	};
}
