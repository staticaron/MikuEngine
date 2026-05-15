#include "Rendering/FrameBuffers/GameFBO.h"

#include "glad/glad.h"

#include "Application.h"
#include "Logger.h"

namespace MikuEngine
{
	void GameFBO::Init()
	{
		glGenFramebuffers( 1, &m_RendererID );
		Bind();

		glGenTextures( 1, &m_ColorTextureID );
		glBindTexture( GL_TEXTURE_2D, m_ColorTextureID );

		auto viewPortSize = Application::GetDataContainer().GetGameResolution();
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, viewPortSize.x, viewPortSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0 );

		// DEPTH AND STENCIL RENDER BUFFER
		glGenRenderbuffers( 1, &m_DepthStencilRenderBufferID );
		glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferID );

		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortSize.x, viewPortSize.y );

		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderBufferID );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) MIKU_CORE_WARN( "FrameBuffer is not ready!" );

		UnBind();

		m_AspectRatio = static_cast<float>( viewPortSize.x ) / viewPortSize.y;
	}

	void GameFBO::Bind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID );
		glViewport( 0, 0, Application::GetDataContainer().GetGameResolution().x, Application::GetDataContainer().GetGameResolution().y );
	}

	void GameFBO::ResizeBufferTexture( glm::vec2 gameResolution )
	{
		Application::GetDataContainer().SetGameResolutionSize( gameResolution );

		glBindTexture( GL_TEXTURE_2D, m_ColorTextureID );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, gameResolution.x, gameResolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferID );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, gameResolution.x, gameResolution.y );
	}

	void GameFBO::UnBind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void GameFBO::Destroy()
	{
		glDeleteFramebuffers( 1, &m_RendererID );
		glDeleteTextures( 1, &m_ColorTextureID );
		glDeleteRenderbuffers( 1, &m_DepthStencilRenderBufferID );
	}
}
