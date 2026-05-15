#include "Systems/MeshRendererSystem.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Helpers/ImGuiHelper.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMesh( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData )
	{
		RenderMeshByBlendMode( scene, appLevelStuff, cameraData, MaterialBlendMode::OPAQUE );
		RenderMeshByBlendMode( scene, appLevelStuff, cameraData, MaterialBlendMode::TRANSPARENT );
	}

	void MeshRendererSystem::RenderMeshByBlendMode( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData, const MaterialBlendMode& blendMode )
	{
		const auto& entities = scene.GetRegistry().view<IDComponent, MeshRendererComponent>();

		const auto& renderer = appLevelStuff.GetRenderer();
		auto& materialManager = appLevelStuff.GetAssetPoolManager().GetMaterialManager();
		auto& modelManager = appLevelStuff.GetAssetPoolManager().GetModelManager();

		// DISABLE WRITING TO DEPTH BUFFER WHEN RENDERING TRANSPARENT MESHES
		if ( blendMode == MaterialBlendMode::TRANSPARENT ) renderer.DisableWriteToDepthBuffer();

		for ( const auto& [ entity, idC, meshRendererC ] : entities.each() )
		{
			const auto& modelUUID = meshRendererC.ModelIdentifier;

			if ( modelUUID.has_value() == false ) continue;

			const auto& model = modelManager.GetModel( meshRendererC.ModelIdentifier.value() );

			Material* material = nullptr;

			if ( meshRendererC.MaterialIdentifier.has_value() == false ) continue;

			auto materialContainer = materialManager.GetMaterial( meshRendererC.MaterialIdentifier.value() );
			if ( materialContainer.has_value() == false ) continue;

			material = &materialContainer.value()->material;

			if ( material->GetBlendMode() != blendMode ) continue;

			material->Bind();

			auto shader = material->GetShader();

			if ( shader.has_value() == false ) return;

			const auto& transform = scene.GetRegistry().get<TransformComponent>( entity );
			glm::mat4 modelMatrix = transform.GetModelMatrix();
			shader.value()->shader.SetUniform<glm::mat4>( "u_Model", modelMatrix );

			auto* stencilReaderC = scene.GetRegistry().try_get<StencilReaderComponent>( entity );
			auto* stencilWriterC = scene.GetRegistry().try_get<StencilWriterComponent>( entity );

			if ( stencilReaderC )
			{
				// enable stencil testing
				glEnable( GL_STENCIL_TEST );

				// read from stencil and pass the test to render
				glStencilFunc( GL_EQUAL, stencilReaderC->ReadValue, 0xFF );

				glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
			}

			if ( stencilWriterC )
			{
				// enable stencil testing
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
			if ( stencilWriterC || stencilReaderC ) glDisable( GL_STENCIL_TEST );
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
