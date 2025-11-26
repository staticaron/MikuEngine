#include "Systems/RenderingSystem.h"

#include "AppLevelStuff.h"
#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	void RenderingSystem::RenderSprite( const entt::registry& registry, AppLevelStuff& appLevelStuff )
	{
		const auto& spriteRenderers = registry.view<TransformComponent, SpriteRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = appLevelStuff.GetTextureManager();

		auto shader = quad.GetShader();
		shader.Bind();

		for ( const auto& [ entity, transform, spriteRenderer ] : spriteRenderers.each() )
		{
			const Texture& texture = textureManager.GetTexture( "miku" );
			texture.Bind( 0 );

			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			renderer.Draw( quad.GetVA(), quad.GetIB(), quad.GetShader() );
		}
	};
}
