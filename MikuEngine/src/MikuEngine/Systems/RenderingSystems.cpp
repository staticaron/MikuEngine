#include "Systems/RenderingSystem.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLevelStuff.h"
#include "Application.h"
#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	void RenderingSystem::RenderSprite( const entt::registry& registry, AppLevelStuff& appLevelStuff )
	{
		const auto& entities = registry.view<TransformComponent, SpriteRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = appLevelStuff.GetTextureManager();

		auto shader = quad.GetShader();
		shader.Bind();

		for ( const auto& [ entity, transform, spriteRenderer ] : entities.each() )
		{
			const Texture& texture = textureManager.GetTexture( spriteRenderer.TextureIdentifier );
			texture.Bind( 0 );
			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			auto viewport = Application::GetApplication()->GetDataContainer().GetViewportSize();

			glm::mat4 proj = glm::ortho( 0.0f, viewport.x, viewport.y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 view = glm::mat4( 1.0f );
			glm::mat4 model = transform.GetModelMatrix();

			glm::mat4 mvp = proj * view * model;

			shader.SetUniform<glm::mat4>( "u_MVP", mvp );

			renderer.Draw( quad.GetVA(), quad.GetIB(), quad.GetShader() );
		}
	};
}
