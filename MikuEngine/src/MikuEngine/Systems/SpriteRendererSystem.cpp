#include "Systems/SpriteRendererSystem.h"

#include <optional>

#include "imgui.h"

#include "AppLevelStuff.h"
#include "Components.h"
#include "Data/CameraData.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"
#include "Helpers/SerializationHelper.h"
#include "Logger.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void SpriteRendererSystem::RenderSprite( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		const auto& entities = scene.GetRegistry().view<DataComponent, SpriteRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& quad = renderer.GetQuad();
		const TextureManager& textureManager = appLevelStuff.GetAssetPoolManager().GetTextureManager();
		const ShaderManager& shaderManager = appLevelStuff.GetAssetPoolManager().GetShaderManager();
		MaterialManager& materialManager = appLevelStuff.GetAssetPoolManager().GetMaterialManager();

		for ( const auto& [ entity, data, spriteRenderer ] : entities.each() )
		{
			Material* material = nullptr;

			if ( spriteRenderer.MaterialUUID.has_value() )
			{
				auto materialContainer = materialManager.GetMaterial( spriteRenderer.MaterialUUID.value() );
				if ( materialContainer.has_value() == false )
				{
					MIKU_CORE_WARN( "Material assigned to this sprite renderer is not loaded!" );
					return;
				}

				material = &materialContainer.value()->material;
			}
			else
			{
				MIKU_CORE_ERROR( "Material for this Sprite Renderer Not Found!" );
				return;
			}

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			shader.value()->shader.SetUniform<glm::mat4>( "u_Model", modelMatrix );

			renderer.Draw( quad.GetVA(), quad.GetIB(), shader.value()->shader );
		}
	};

	void SpriteRendererSystem::SpriteRendererComponentRenderImGui( Entity entity, SpriteRendererComponent& spriteRendererC, std::function<void()> textureEditBtnCallback, std::function<void()> materialEditBtnCallback )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "SpriteRendererComponent", &keep ) )
		{
			ImGuiHelper::RenderDragableTextureInput( spriteRendererC.TextureIdentifier, textureEditBtnCallback );
			ImGuiHelper::RenderDragableMaterialInput( spriteRendererC.MaterialUUID, materialEditBtnCallback );

			ImGui::DragFloat4( "Tint", &spriteRendererC.Tint.x );
		};

		if ( !keep ) entity.RemoveComponent<SpriteRendererComponent>();
	}

	void SpriteRendererSystem::SerializeSpriteRendererComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto spriteRenderer = entity.GetReadOnlyComponent<SpriteRendererComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "SpriteRendererComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "texture" << YAML::Value << spriteRenderer.TextureIdentifier.value();
		emitter << YAML::Key << "material" << YAML::Value << ( spriteRenderer.MaterialUUID.has_value() ? spriteRenderer.MaterialUUID.value() : UUID( 0 ) );
		emitter << YAML::Key << "tint" << YAML::Value << YAML::Flow << YAML::BeginSeq << spriteRenderer.Tint.x << spriteRenderer.Tint.y << spriteRenderer.Tint.z << spriteRenderer.Tint.w << YAML::EndSeq;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void SpriteRendererSystem::DeSerializeSpriteRendererComponent( SpriteRendererComponent& spriteRendererC, const YAML::Node& node )
	{
		std::string texture = node[ "texture" ].as<std::string>();
		std::string material = node[ "material" ].as<std::string>();

		glm::vec4 tint;
		DecodeVec4( node[ "tint" ], tint );

		spriteRendererC.TextureIdentifier = texture.empty() ? std::optional<UUID>( std::nullopt ) : UUID( texture );
		spriteRendererC.MaterialUUID = material.empty() ? std::optional<UUID>( std::nullopt ) : UUID( material );
		spriteRendererC.Tint = tint;
	}
}
