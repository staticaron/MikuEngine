#include "Components/SpriteRenderer.h"

#include "GameObjects/Camera.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	SpriteRenderer::SpriteRenderer() : m_FilePath( RESOURCE_DIR "textures/base.png" )
	{
		m_Quad.Init();
	}

	SpriteRenderer::SpriteRenderer( const std::string& filePath ) : m_FilePath( filePath ) {}

	void SpriteRenderer::Render( const Renderer& renderer, const TextureManager& textureManager, const Camera& camera ) const
	{
		auto texture = textureManager.GetTexture( "miku" );
		texture.Bind( 0 );

		auto shader = m_Quad.GetShader();
		shader.SetUniform<unsigned int>( "u_Tex", 0 );
		shader.SetUniform<glm::mat4>( "u_MVP", camera.GetMVPFromModelMatrix( glm::mat4( 1.0f ) ) );

		renderer.Draw( m_Quad.GetVA(), m_Quad.GetIB(), m_Quad.GetShader() );
	}
}
