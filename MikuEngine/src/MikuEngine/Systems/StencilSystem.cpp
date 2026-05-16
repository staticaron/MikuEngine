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

	void StencilSystem::SerializeStencilReader( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto stencilReaderC = entity.GetReadOnlyComponent<StencilReaderComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "StencilReaderComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "read-value" << YAML::Value << static_cast<int>( stencilReaderC.ReadValue );
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void StencilSystem::DeSerializeStencilReader( StencilReaderComponent& stencilReaderC, const YAML::Node& node )
	{
		uint8_t stencilReadValue = node[ "read-value" ].as<uint8_t>();
		stencilReaderC.ReadValue = stencilReadValue;
	}

	void StencilSystem::StencilWriterRenderImGui( Entity entity, StencilWriterComponent& stencilWriterC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "StencilWriterComponent", &keep ) )
		{
			ImGui::DragScalar( "Write Value", ImGuiDataType_U8, &stencilWriterC.WriteValue, 1 );
		};

		if ( !keep ) entity.RemoveComponent<StencilWriterComponent>();
	}

	void StencilSystem::SerializeStencilWriter( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto stencilReaderC = entity.GetReadOnlyComponent<StencilWriterComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "StencilWriterComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "write-value" << YAML::Value << static_cast<int>( stencilReaderC.WriteValue );
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void StencilSystem::DeSerializeStencilWriter( StencilWriterComponent& stencilWriterC, const YAML::Node& node )
	{
		uint8_t stencilWriteValue = node[ "write-value" ].as<uint8_t>();
		stencilWriterC.WriteValue = stencilWriteValue;
	}
}
