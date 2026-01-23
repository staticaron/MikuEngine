#include "Systems/RenderingSystem.h"

#include <optional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLevelStuff.h"
#include "Application.h"
#include "Components.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void RenderingSystem::RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		// Quads To Render
		const auto& entities = scene.GetRegistry().view<DataComponent, SpriteRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = appLevelStuff.GetTextureManager();

		auto shader = quad.GetShader();
		shader.Bind();

		for ( const auto& [ entity, data, spriteRenderer ] : entities.each() )
		{
			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );

			if ( !spriteRenderer.TextureIdentifier.has_value() ) continue;

			const Texture& texture = textureManager.GetTexture( spriteRenderer.TextureIdentifier.value() );
			texture.Bind( 0 );
			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			glm::mat4 proj = glm::ortho( 0.0f, cameraData.cameraResolution.x, cameraData.cameraResolution.y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 view = cameraData.viewMatrix;
			glm::mat4 model = transform.GetModelMatrix();

			glm::mat4 mvp = proj * view * model;

			shader.SetUniform<glm::mat4>( "u_MVP", mvp );

			renderer.Draw( quad.GetVA(), quad.GetIB(), quad.GetShader() );
		}
	};
}
