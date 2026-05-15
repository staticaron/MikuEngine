#include "Rendering/FrameBuffers/SceneFBO.h"

#include "Application.h"
#include "glad/glad.h"
#include "Logger.h"

namespace MikuEngine
{
	void SceneFBO::Init()
	{
		glGenFramebuffers( 1, &m_RendererID );
		Bind();

		glGenTextures( 1, &m_ColorTextureID );
		glBindTexture( GL_TEXTURE_2D, m_ColorTextureID );

		auto viewPortSize = Application::GetDataContainer().GetViewportSize();
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, viewPortSize.x, viewPortSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0 );

		// DEPTH AND STENCIL TEXTURE
		glGenRenderbuffers( 1, &m_DepthStencilRenderBufferID );
		glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferID );

		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortSize.x, viewPortSize.y );

		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderBufferID );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) MIKU_CORE_WARN( "FrameBuffer is not ready!" );

		UnBind();
	}

	void SceneFBO::Bind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID );
		glViewport( 0, 0, Application::GetDataContainer().GetViewportSize().x, Application::GetDataContainer().GetViewportSize().y );
	}

	void SceneFBO::UnBind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void SceneFBO::Destroy()
	{
		glDeleteFramebuffers( 1, &m_RendererID );
		glDeleteTextures( 1, &m_ColorTextureID );
		glDeleteRenderbuffers( 1, &m_DepthStencilRenderBufferID );
	}

	void SceneFBO::ResizeBufferTexture( glm::vec2 viewportSize )
	{
		Application::GetDataContainer().SetViewportSize( viewportSize );

		// Resize the color texture
		glBindTexture( GL_TEXTURE_2D, m_ColorTextureID );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, viewportSize.x, viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		// Resize the depth and stencil buffers
		glBindRenderbuffer( GL_RENDERBUFFER, m_DepthStencilRenderBufferID );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y );
	}
}
