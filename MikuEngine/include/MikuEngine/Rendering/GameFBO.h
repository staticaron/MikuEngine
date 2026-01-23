#pragma once

#include "Core.h"

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

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_TextureID = 0;

		float m_AspectRatio = 1.0f;
	};
}
