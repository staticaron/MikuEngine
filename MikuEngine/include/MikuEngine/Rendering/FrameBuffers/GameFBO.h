#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace MikuEngine
{
	class MIKU_API GameFBO
	{
	public:
		void Init();

		void Bind() const;
		void UnBind() const;

		void Destroy();

		unsigned int GetTextureID() const { return m_ColorTextureID; }

		float GetAspectRatio() const { return m_AspectRatio; }

		void ResizeBufferTexture( glm::vec2 gameResolution );

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_ColorTextureID = 0;
		unsigned int m_DepthTextureID = 0;

		float m_AspectRatio = 1.0f;
	};
}
