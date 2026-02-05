#include "Systems/RenderingSystem.h"

#include <optional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLevelStuff.h"
#include "Components.h"
#include "Entity.h"
#include "Helpers/SerializationHelper.h"
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

	void RenderingSystem::ClearColor( glm::vec4 color )
	{
		glClearColor( color.x, color.y, color.z, color.w );
		glClear( GL_COLOR_BUFFER_BIT );
	}

	void RenderingSystem::SerializeSpriteRendererComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto spriteRenderer = entity.GetReadOnlyComponent<SpriteRendererComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "SpriteRendererComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "texture" << YAML::Value << spriteRenderer.TextureIdentifier.value();
		emitter << YAML::Key << "tint" << YAML::Value << YAML::Flow << YAML::BeginSeq << spriteRenderer.Tint.x << spriteRenderer.Tint.y << spriteRenderer.Tint.z << spriteRenderer.Tint.w << YAML::EndSeq;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void RenderingSystem::DeSerializeSpriteRendererComponent( SpriteRendererComponent& spriteRendererC, const YAML::Node& node )
	{
		std::string texture = node[ "texture" ].as<std::string>();

		glm::vec4 tint;
		DecodeVec4( node[ "tint" ], tint );

		spriteRendererC.TextureIdentifier = texture.empty() ? std::optional<UUID>( std::nullopt ) : UUID( texture );
		spriteRendererC.Tint = tint;
	}
}
