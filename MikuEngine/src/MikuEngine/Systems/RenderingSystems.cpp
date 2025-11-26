#include "Managers/TextureManager.h"
#include "Rendering/Texture.h"
#include "Systems/RenderingSystem.h"

#include "Application.h"
#include "Components/SpriteRenderer.h"

namespace MikuEngine
{
	void RenderingSystem::RenderSprite( const entt::registry& registry )
	{
		const auto& spriteRenderers = registry.view<SpriteRendererComponent>();

		const auto& renderer = Application::GetApplication()->GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = Application::GetApplication()->GetTextureManager();

		auto shader = quad.GetShader();
		shader.Bind();

		for ( const auto& spriteRenderer : spriteRenderers )
		{
			const Texture& texture = textureManager.GetTexture( "miku" );
			texture.Bind( 0 );

			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			renderer.Draw( quad.GetVA(), quad.GetIB(), quad.GetShader() );
		}
	};
}
