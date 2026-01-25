#include "Rendering/GameFBO.h"

#include "glad/glad.h"

#include "Application.h"
#include "Logger.h"

namespace MikuEngine
{
	void GameFBO::Init()
	{
		glGenFramebuffers( 1, &m_RendererID );
		Bind();

		glGenTextures( 1, &m_TextureID );
		glBindTexture( GL_TEXTURE_2D, m_TextureID );

		auto viewPortSize = Application::GetDataContainer().GetGameResolution();
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, viewPortSize.x, viewPortSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glBindTexture( GL_TEXTURE_2D, 0 );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0 );

		if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE ) MIKU_CORE_WARN( "FrameBuffer is not ready!" );

		UnBind();

		m_AspectRatio = viewPortSize.x / viewPortSize.y;
	}

	GameFBO::~GameFBO()
	{
		Destroy();
	}

	void GameFBO::Bind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_RendererID );
		glViewport( 0, 0, Application::GetDataContainer().GetGameResolution().x, Application::GetDataContainer().GetGameResolution().y );
	}

	void GameFBO::ResizeBufferTexture( glm::vec2 gameResolution )
	{
		Application::GetDataContainer().SetGameResolutionSize( gameResolution );

		Destroy();
		Init();
	}

	void GameFBO::UnBind() const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	void GameFBO::Destroy()
	{
		glDeleteFramebuffers( 1, &m_RendererID );
		glDeleteTextures( 1, &m_RendererID );
	}
}
