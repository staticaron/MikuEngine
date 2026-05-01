#include "Systems/MeshRendererSystem.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Helpers/ImGuiHelper.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMesh( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		const auto& entities = scene.GetRegistry().view<DataComponent, MeshRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		auto& materialManager = appLevelStuff.GetAssetPoolManager().GetMaterialManager();
		auto& modelManager = appLevelStuff.GetAssetPoolManager().GetModelManager();

		for ( const auto& [ entity, dataC, meshRendererC ] : entities.each() )
		{
			const auto& modelUUID = meshRendererC.ModelIdentifier;

			if ( modelUUID.has_value() == false ) continue;

			const auto& model = modelManager.GetModel( meshRendererC.ModelIdentifier.value() );

			Material* material = nullptr;

			if ( meshRendererC.MaterialIdentifier.has_value() )
			{
				auto materialContainer = materialManager.GetMaterial( meshRendererC.MaterialIdentifier.value() );
				material = &materialContainer.value()->material;
			}
			else
				continue;

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			shader.value()->shader.SetUniform<glm::mat4>( "u_Model", modelMatrix );

			// Render all the meshes in the model
			for ( const auto& mesh : model.model.GetMeshes() )
			{
				renderer.Draw( mesh.GetVA(), mesh.GetIB(), shader.value()->shader );
			}
		}
	}

	void MeshRendererSystem::MeshRendererComponentRenderImGui( Entity entity, MeshRendererComponent& meshRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "MeshRendererComponent", &keep ) )
		{
			ImGuiHelper::RenderDragableModelInput( "Model", meshRendererC.ModelIdentifier, modelEditBtnCallback );
			ImGuiHelper::RenderDragableMaterialInput( "Material", meshRendererC.MaterialIdentifier, materialEditBtnCallback );
		}

		if ( !keep ) entity.RemoveComponent<MeshRendererComponent>();
	}

	void MeshRendererSystem::SerializeMeshRendererComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto meshRenderer = entity.GetReadOnlyComponent<MeshRendererComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "MeshRendererComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "model" << YAML::Value << meshRenderer.ModelIdentifier.value();
		emitter << YAML::Key << "material" << YAML::Value << ( meshRenderer.MaterialIdentifier.has_value() ? meshRenderer.MaterialIdentifier.value().ToString() : "<NONE>" );
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void MeshRendererSystem::DeSerializeMeshRendererComponent( MeshRendererComponent& modelRendererC, const YAML::Node& node )
	{
		std::string model = node[ "model" ].as<std::string>();
		std::string material = node[ "material" ].as<std::string>();

		if ( model.empty() == false )
		{
			if ( model == "<NONE>" )
				modelRendererC.ModelIdentifier = std::nullopt;
			else
				modelRendererC.ModelIdentifier = UUID( model );
		}

		if ( material.empty() == false )
		{
			if ( material == "<NONE>" )
				modelRendererC.MaterialIdentifier = std::nullopt;
			else
				modelRendererC.MaterialIdentifier = UUID( material );
		}
	}
}
