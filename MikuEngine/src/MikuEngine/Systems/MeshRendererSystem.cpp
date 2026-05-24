#include "Systems/MeshRendererSystem.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Helpers/ImGuiHelper.h"
#include "Logger.h"
#include "Scene/Scene.h"
#include "Systems/TransformSystems.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMeshByType( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData, MaterialBlendMode mode )
	{
		auto cameraTransform = cameraData.GetProjViewMatrix();
		glm::vec3 cameraPosition = cameraTransform[ 3 ];

		std::vector<DistancedEntity> distancedEntities;

		const auto& entities = scene.GetRegistry().view<MeshRendererComponent>();

		for ( const auto& [ entt, meshRendererC ] : entities.each() )
		{
			auto& materialUUID = meshRendererC.MaterialIdentifier;
			if ( materialUUID.has_value() == false ) continue;

			auto material = appLevelStuff.GetAssetPoolManager().GetMaterialManager().GetMaterial( materialUUID.value() );
			if ( material.has_value() == false ) continue;

			if ( material.value()->material.GetRenderOrder().mode != mode ) continue;

			auto transformMtx = TransformSystem::GetTransformMatrix( scene, entt );

			float distanceFromCamera = glm::length( cameraPosition - glm::vec3( transformMtx[ 3 ] ) );

			distancedEntities.push_back( { entt, &meshRendererC, material.value(), transformMtx, distanceFromCamera } );
		}

		std::sort( distancedEntities.begin(), distancedEntities.end(), []( const DistancedEntity& a, const DistancedEntity& b ) {
			if ( a.materialContainer->material.GetRenderOrder().order == b.materialContainer->material.GetRenderOrder().order )
				return a.distanceFromCamera >= b.distanceFromCamera;
			else
				return a.materialContainer->material.GetRenderOrder().order <= b.materialContainer->material.GetRenderOrder().order;
		} );

		RenderMeshByBlendMode( scene, appLevelStuff, distancedEntities, cameraData, mode );
	}

	void MeshRendererSystem::RenderMeshByBlendMode( const Scene& scene, AppLevelStuff& appLevelStuff, const std::vector<DistancedEntity>& distancedEntities, const CameraData& cameraData, const MaterialBlendMode& blendMode )
	{
		const auto& renderer = appLevelStuff.GetRenderer();
		auto& materialManager = appLevelStuff.GetAssetPoolManager().GetMaterialManager();
		auto& modelManager = appLevelStuff.GetAssetPoolManager().GetModelManager();

		// DISABLE WRITING TO DEPTH BUFFER WHEN RENDERING TRANSPARENT MESHES
		if ( blendMode == MaterialBlendMode::TRANSPARENT ) renderer.DisableWriteToDepthBuffer();

		for ( const auto& distancedEntity : distancedEntities )
		{
			const auto& modelUUID = distancedEntity.meshRendererC->ModelIdentifier;
			if ( modelUUID.has_value() == false ) continue;
			auto model = modelManager.GetModel( distancedEntity.meshRendererC->ModelIdentifier.value() );
			if ( model.has_value() == false ) continue;

			Material material = distancedEntity.materialContainer->material;

			material.Bind();

			auto shader = material.GetShader();

			if ( shader.has_value() == false ) continue;

			shader.value()->shader.SetUniform<glm::mat4>( "u_Model", distancedEntity.transformMatrix );

			auto* stencilReaderC = scene.GetRegistry().try_get<StencilReaderComponent>( distancedEntity.entt );
			auto* stencilWriterC = scene.GetRegistry().try_get<StencilWriterComponent>( distancedEntity.entt );

			if ( stencilReaderC )
			{
				glEnable( GL_STENCIL_TEST );
				glDisable( GL_DEPTH_TEST );

				// read from stencil and pass the test to render
				glStencilFunc( GL_EQUAL, stencilReaderC->ReadValue, 0xFF );

				glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

				glEnable( GL_DEPTH_TEST );
			}

			if ( stencilWriterC )
			{
				glEnable( GL_STENCIL_TEST );

				// write to the values
				glStencilMask( 0xFF );

				// Always pass the stencil test, all pixels for the upcoming renders will lead to write values
				glStencilFunc( GL_ALWAYS, stencilWriterC->WriteValue, 0xFF );

				// If the pixel is hidden behind some other object we dont write to stencil buffer
				glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
			}

			// Render all the meshes in the model
			for ( const auto& mesh : model.value()->model.GetMeshes() )
			{
				renderer.Draw( mesh.GetVA(), mesh.GetIB(), shader.value()->shader );
			}

			// Disable the stencil testing if it was opened by stencil reader / writers before
			if ( stencilWriterC || stencilReaderC )
			{
				glDisable( GL_STENCIL_TEST );
				glEnable( GL_DEPTH_TEST );
			};
		}

		if ( blendMode == MaterialBlendMode::TRANSPARENT ) renderer.EnableWriteToDepthBuffer();
	}

	void MeshRendererSystem::MeshRendererComponentRenderImGui( Entity entity, MeshRendererComponent& meshRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "MeshRendererComponent", &keep ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderDragableModelInput( "Model", meshRendererC.ModelIdentifier, modelEditBtnCallback );
			ImGuiHelper::RenderDragableMaterialInput( "Material", meshRendererC.MaterialIdentifier, materialEditBtnCallback );

			ImGuiHelper::EndPropertyTable();
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
