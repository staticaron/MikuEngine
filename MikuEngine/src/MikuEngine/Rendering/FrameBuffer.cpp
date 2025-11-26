#include "Rendering/FrameBuffer.h"

#include "Application.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"

namespace MikuEngine
{
	void FrameBuffer::Init()
	{
		glGenFramebuffers( 1, &m_RendererID );
		Bind();

		glGenTextures( 1, &m_TextureID );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		auto viewPortSize = Application::GetDataContainer().m_ViewportSize;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, viewPortSize.x, viewPortSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0 );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) spdlog::warn( "FrameBuffer is not ready!" );

		UnBind();
	}

	FrameBuffer::~FrameBuffer()
	{
		Destroy();
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID );
		glViewport( 0, 0, Application::GetDataContainer().m_ViewportSize.x, Application::GetDataContainer().m_ViewportSize.y );
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void FrameBuffer::Destroy()
	{
		glDeleteFramebuffers( 1, &m_RendererID );
		glDeleteTextures( 1, &m_RendererID );
	}

	void FrameBuffer::ResizeFrameBufferTexture( glm::vec2 viewportSize )
	{
		spdlog::info( "Changing the Viewport Size from ({}, {}) to ({}, {})", Application::GetDataContainer().m_ViewportSize.x, Application::GetDataContainer().m_ViewportSize.y, viewportSize.x, viewportSize.y );
		Application::GetDataContainer().m_ViewportSize = viewportSize;

		Destroy();
		Init();
	}

}
