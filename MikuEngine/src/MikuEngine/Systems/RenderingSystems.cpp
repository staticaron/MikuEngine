#include "Logger.h"
#include "Systems/RenderingSystem.h"

#include <optional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLevelStuff.h"
#include "Application.h"
#include "Components/CameraComponent.h"
#include "Components/DataComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

namespace MikuEngine
{
	void RenderingSystem::RenderSprite( const entt::registry& registry, AppLevelStuff& appLevelStuff )
	{
		// Quads To Render
		const auto& entities = registry.view<DataComponent, SpriteRendererComponent>();

		// Camera
		const auto& cameras = registry.view<CameraComponent>();

		std::optional<entt::entity> cameraEntity;
		std::optional<CameraComponent> cameraComponent;

		for ( const auto& [ entity, cam ] : cameras.each() )
		{
			cameraEntity = entity;
			cameraComponent = cam;
		}

		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = appLevelStuff.GetTextureManager();

		auto shader = quad.GetShader();
		shader.Bind();

		for ( const auto& [ entity, data, spriteRenderer ] : entities.each() )
		{
			// INFO: No point in doing rendering if there is no camera
			if ( !cameraEntity.has_value() )
			{
				MIKU_WARN( "There is no ACTIVE camera in this scene" );
				return;
			}
			const auto& transform = registry.get<TransformComponent>( entity );

			if ( spriteRenderer.TextureIdentifier == "" ) continue;

			const Texture& texture = textureManager.GetTexture( spriteRenderer.TextureIdentifier );
			texture.Bind( 0 );
			shader.SetUniform<unsigned int>( "u_Tex", 0 );

			auto viewport = Application::GetApplication()->GetDataContainer().GetViewportSize();

			glm::mat4 proj = glm::ortho( 0.0f, viewport.x, viewport.y, 0.0f, -1000.0f, 1000.0f );
			glm::mat4 view = cameraComponent.value().GetViewMatrix( registry, cameraEntity.value() );
			glm::mat4 model = transform.GetModelMatrix();

			glm::mat4 mvp = proj * view * model;

			shader.SetUniform<glm::mat4>( "u_MVP", mvp );

			renderer.Draw( quad.GetVA(), quad.GetIB(), quad.GetShader() );
		}
	};
}
