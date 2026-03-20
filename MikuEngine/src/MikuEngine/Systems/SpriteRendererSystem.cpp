#include "Systems/SpriteRendererSystem.h"

#include <optional>

#include "imgui.h"

#include "AppLevelStuff.h"
#include "Application.h"
#include "Components.h"
#include "Data/CameraData.h"
#include "Entity.h"
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

				if ( materialContainer.has_value() )
					material = materialContainer->material;
				else
					MIKU_CORE_ERROR( "Material for this Sprite Renderer Not Found!" );
			}

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			shader.value()->SetUniform<glm::mat4>( "u_Model", modelMatrix );

			renderer.Draw( quad.GetVA(), quad.GetIB(), *shader.value() );
		}
	};

	void SpriteRendererSystem::SpriteRendererComponentRenderImGui( Entity entity, SpriteRendererComponent& spriteRendererC, std::function<void()> textureEditBtnCallback, std::function<void()> shaderEditBtnCallback )
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();
		auto& materialManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager();

		bool keep = true;

		if ( ImGui::CollapsingHeader( "SpriteRendererComponent", &keep ) )
		{
			auto textureUUID = spriteRendererC.TextureIdentifier;

			std::string textureName = "NONE";

			if ( textureUUID.has_value() )
			{
				// TODO: Combine Texture and TextureName Into one TextureContainer
				auto texture = textureManager.GetTexture( textureUUID.value() );
				textureName = textureManager.GetTextureName( textureUUID.value() );
			}

			DISABLED_IMGUI( ImGui::Button( textureName.c_str() ) );
			ImGui::SameLine();
			if ( ImGui::Button( "EDIT...##texture" ) ) textureEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto texturePath = static_cast<const char*>( payload->Data );
					spriteRendererC.TextureIdentifier = textureManager.GetTextureByFilePath( texturePath )->GetUUID();
				}

				ImGui::EndDragDropTarget();
			}

			auto materialUUID = spriteRendererC.MaterialUUID;
			std::string materialName = "<NONE>";

			if ( materialUUID.has_value() )
			{
				auto material = materialManager.GetMaterial( materialUUID.value() );
				if ( material.has_value() ) materialName = material->name;
			}

			DISABLED_IMGUI( ImGui::Button( materialName.c_str() ) );
			ImGui::SameLine();
			if ( ImGui::Button( "EDIT...##shader" ) ) shaderEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "MATERIAL_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto materialPath = static_cast<const char*>( payload->Data );
					auto material = materialManager.GetMaterialByFilePath( materialPath );
					if ( material.has_value() ) spriteRendererC.MaterialUUID = material.value()->GetUUID();
				}

				ImGui::EndDragDropTarget();
			}

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
