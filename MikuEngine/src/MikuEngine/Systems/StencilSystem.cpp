#include "Systems/StencilSystem.h"

#include "imgui.h"

#include "Components/StencilReaderComponent.h"
#include "Components/StencilWriterComponent.h"
#include "Entity.h"

namespace MikuEngine
{
	void StencilSystem::StencilSystem::StencilReaderRenderImGui( Entity entity, StencilReaderComponent& stencilReaderC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "StencilReaderComponent", &keep ) )
		{
			ImGui::DragScalar( "Read Value", ImGuiDataType_U8, &stencilReaderC.ReadValue, 1 );
		};

		if ( !keep ) entity.RemoveComponent<StencilReaderComponent>();
	}

	void StencilSystem::SerializeStencilReader( const Entity& entity, YAML::Emitter& emitter ) {}
	void StencilSystem::DeSerializeStencilReader( StencilReaderComponent& spriteRendererC, const YAML::Node& node ) {}

	void StencilSystem::StencilWriterRenderImGui( Entity entity, StencilWriterComponent& stencilWriterC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "StencilWriterComponent", &keep ) )
		{
			ImGui::DragScalar( "Write Value", ImGuiDataType_U8, &stencilWriterC.WriteValue, 1 );
		};

		if ( !keep ) entity.RemoveComponent<StencilWriterComponent>();
	}

	void StencilSystem::SerializeStencilWriter( const Entity& entity, YAML::Emitter& emitter ) {}
	void StencilSystem::DeSerializeStencilWriter( StencilWriterComponent& spriteRendererC, const YAML::Node& node ) {}
}
