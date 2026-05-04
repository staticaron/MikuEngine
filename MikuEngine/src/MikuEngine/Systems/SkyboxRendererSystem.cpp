#include "Systems/SkyboxRendererSystem.h"

#include "glad/glad.h"

#include "AppLevelStuff.h"
#include "Components/DataComponent.h"
#include "Components/SkyboxComponent.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"
#include "Logger.h"

namespace MikuEngine
{
	void SkyboxRendererSystem::RenderSkybox( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		const auto& entities = scene.GetRegistry().view<DataComponent, SkyboxComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		auto& materialManager = appLevelStuff.GetAssetPoolManager().GetMaterialManager();
		auto& modelManager = appLevelStuff.GetAssetPoolManager().GetModelManager();

		for ( const auto& [ entity, dataC, meshRendererC ] : entities.each() )
		{
			const auto& modelUUID = meshRendererC.ModelIdentifier;

			if ( modelUUID.has_value() == false ) continue;

			const auto& model = modelManager.GetModel( meshRendererC.ModelIdentifier.value() );

			Material* material = nullptr;

			if ( meshRendererC.MaterialIdentifier.has_value() == false ) continue;

			auto materialContainer = materialManager.GetMaterial( meshRendererC.MaterialIdentifier.value() );
			if ( materialContainer.has_value() == false ) continue;

			material = &materialContainer.value()->material;

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			shader.value()->shader.SetUniform<glm::mat4>( "u_Model", modelMatrix );

			glCullFace( GL_FRONT );
			glDepthFunc( GL_LEQUAL );

			// Render all the meshes in the model
			for ( const auto& mesh : model.model.GetMeshes() )
			{
				renderer.Draw( mesh.GetVA(), mesh.GetIB(), shader.value()->shader );
			}

			glDepthFunc( GL_LESS );
			glCullFace( GL_BACK );
		}
	}

	void SkyboxRendererSystem::SkyboxComponentRenderImGui( Entity entity, SkyboxComponent& skyboxC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "SkyboxComponent", &keep ) )
		{
			ImGuiHelper::RenderDragableModelInput( "Skybox Model", skyboxC.ModelIdentifier, modelEditBtnCallback );
			ImGuiHelper::RenderDragableMaterialInput( "Skybox Material", skyboxC.MaterialIdentifier, materialEditBtnCallback );
		};

		if ( !keep ) entity.RemoveComponent<SkyboxComponent>();
	}

	void SkyboxRendererSystem::SerializeSkyboxComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto skyboxC = entity.GetReadOnlyComponent<SkyboxComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "SkyboxComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "model" << YAML::Value << ( skyboxC.ModelIdentifier.has_value() ? skyboxC.ModelIdentifier.value().ToString() : "<NONE>" );
		emitter << YAML::Key << "material" << YAML::Value << ( skyboxC.MaterialIdentifier.has_value() ? skyboxC.MaterialIdentifier.value().ToString() : "<NONE>" );
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void SkyboxRendererSystem::DeSerializeSkyboxComponent( SkyboxComponent& skyboxC, const YAML::Node& node )
	{
		std::string model = node[ "model" ].as<std::string>();
		std::string material = node[ "material" ].as<std::string>();

		if ( model.empty() == false )
		{
			if ( model == "<NONE>" )
				skyboxC.ModelIdentifier = std::nullopt;
			else
				skyboxC.ModelIdentifier = UUID( model );
		}

		if ( material.empty() == false )
		{
			if ( material == "<NONE>" )
				skyboxC.MaterialIdentifier = std::nullopt;
			else
				skyboxC.MaterialIdentifier = UUID( material );
		}
	}
}
