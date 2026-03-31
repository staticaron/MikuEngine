#include "Systems/MeshRendererSystem.h"

#include "Components.h"
#include "Data/CameraData.h"
#include "Helpers/ImGuiHelper.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMesh( const Scene& scene, const CameraData& cameraData ) {}

	void MeshRendererSystem::MeshRendererComponentRenderImGui( Entity entity, MeshRendererComponent& meshRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "MeshRendererComponent", &keep ) )
		{
			ImGuiHelper::RenderDragableModelInput( meshRendererC.Model, modelEditBtnCallback );
			ImGuiHelper::RenderDragableMaterialInput( meshRendererC.Material, materialEditBtnCallback );
		}
	}

	void MeshRendererSystem::SerializeMeshRendererComponent( const Entity& entity, YAML::Emitter& emitter ) {}
	void MeshRendererSystem::DeSerializeMeshRendererComponent( MeshRendererComponent& spriteRendererC, const YAML::Node& node ) {}
}
