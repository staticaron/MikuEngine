#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace MikuEngine
{
	class MIKU_API GameFBO
	{
	public:
		GameFBO() = default;
		~GameFBO();

		void Init();

		void Bind() const;
		void UnBind() const;

		void Destroy();

		unsigned int GetTextureID() const { return m_TextureID; }

		float GetAspectRatio() const { return m_AspectRatio; }

		void ResizeBufferTexture( glm::vec2 gameResolution );

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_TextureID = 0;

		float m_AspectRatio = 1.0f;
	};
}
