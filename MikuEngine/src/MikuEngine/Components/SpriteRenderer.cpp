#include "Components/SpriteRenderer.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	SpriteRenderer::SpriteRenderer() : m_FilePath( RESOURCE_DIR "textures/base.png" )
	{
	}

	SpriteRenderer::SpriteRenderer( const std::string& filePath ) : m_FilePath( filePath )
	{
	}

	void SpriteRenderer::Render( const Renderer& renderer ) const
	{
		renderer.Draw( m_Quad.GetVA(), m_Quad.GetIB(), m_Quad.GetShader() );
	}
}
