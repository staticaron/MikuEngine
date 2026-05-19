#include "Systems/MeshRendererSystem.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Helpers/ImGuiHelper.h"
#include "Logger.h"
#include "Scene/Scene.h"
#include "Systems/TransformSystems.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMesh( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		auto cameraTransform = cameraData.GetProjViewMatrix();
		glm::vec3 cameraPosition = cameraTransform[ 3 ];

		std::vector<DistancedEntity> distancedTransparentEntities;
		std::vector<DistancedEntity> distancedOpaqueEntities;

		const auto& entities = scene.GetRegistry().view<MeshRendererComponent>();

		// Loop through each and every mesh rendering component to get opaque and transparent object lists
		// Create opaque and transparent object lists along with their distance from the camera
		for ( const auto& [ entt, meshRendererC ] : entities.each() )
		{
			auto& materialUUID = meshRendererC.MaterialIdentifier;
			if ( materialUUID.has_value() == false ) return;

			auto material = appLevelStuff.GetAssetPoolManager().GetMaterialManager().GetMaterial( materialUUID.value() );
			if ( material.has_value() == false ) return;

			auto transformMtx = TransformSystem::GetTransformMatrix( scene, entt );

			float distanceFromCamera = glm::length( cameraPosition - glm::vec3( transformMtx[ 3 ] ) );

			if ( material.value()->material.GetBlendMode() == MaterialBlendMode::TRANSPARENT )
			{
				distancedTransparentEntities.push_back( { entt, &meshRendererC, transformMtx, distanceFromCamera } );
			}
			else
				distancedOpaqueEntities.push_back( { entt, &meshRendererC, transformMtx, distanceFromCamera } );
		}

		// sort the transparent objects based on the distance from the camera
		std::sort( distancedTransparentEntities.begin(), distancedTransparentEntities.end(), []( const DistancedEntity& a, const DistancedEntity& b ) { return a.distanceFromCamera >= b.distanceFromCamera; } );

		// Render Opaque first then transparent objects
		RenderMeshByBlendMode( scene, appLevelStuff, distancedOpaqueEntities, cameraData, MaterialBlendMode::OPAQUE );
		RenderMeshByBlendMode( scene, appLevelStuff, distancedTransparentEntities, cameraData, MaterialBlendMode::TRANSPARENT );
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

			const auto& model = modelManager.GetModel( distancedEntity.meshRendererC->ModelIdentifier.value() );

			Material* material = nullptr;

			if ( distancedEntity.meshRendererC->MaterialIdentifier.has_value() == false ) continue;

			auto materialContainer = materialManager.GetMaterial( distancedEntity.meshRendererC->MaterialIdentifier.value() );
			if ( materialContainer.has_value() == false ) continue;

			material = &materialContainer.value()->material;

			if ( material->GetBlendMode() != blendMode ) continue;

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

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
			for ( const auto& mesh : model.model.GetMeshes() )
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
