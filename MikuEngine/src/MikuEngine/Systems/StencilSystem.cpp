#include "Systems/StencilSystem.h"

#include "imgui.h"

#include "glad/glad.h"

#include "Components/StencilReaderComponent.h"
#include "Components/StencilWriterComponent.h"
#include "Entity.h"
#include "Helpers/ImGuiHelper.h"

namespace MikuEngine
{
	void StencilSystem::StartStencilReading( const StencilReaderComponent& stencilReaderC )
	{
		glEnable( GL_STENCIL_TEST );
		glStencilFunc( GL_EQUAL, stencilReaderC.ReadValue, 0xFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilMask( 0x00 );

		if ( stencilReaderC.RespectDepthBuffer == false ) glDisable( GL_DEPTH_TEST );
	}

	void StencilSystem::StopStencilReading( const StencilReaderComponent& stencilReaderC )
	{
		glDisable( GL_STENCIL_TEST );
		glStencilMask( 0xFF );

		if ( stencilReaderC.RespectDepthBuffer == false ) glEnable( GL_DEPTH_TEST );
	}

	void StencilSystem::StencilSystem::StencilReaderRenderImGui( Entity entity, StencilReaderComponent& stencilReaderC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "StencilReaderComponent", &keep ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Read Value", [ & ]() { ImGui::DragScalar( "##Read Value", ImGuiDataType_U8, &stencilReaderC.ReadValue, 1 ); } );
			ImGuiHelper::RenderTableItem( "Respect Depth", [ & ]() { ImGui::Checkbox( "##Respect Depth", &stencilReaderC.RespectDepthBuffer ); } );

			ImGuiHelper::EndPropertyTable();
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

	void StencilSystem::StartStencilWriting( const StencilWriterComponent& stencilWriterC )
	{
		glEnable( GL_STENCIL_TEST );
		glStencilFunc( GL_ALWAYS, stencilWriterC.WriteValue, 0xFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		glStencilMask( 0xFF );
	}

	void StencilSystem::StopStencilWriting( const StencilWriterComponent& stencilWriterC )
	{
		glDisable( GL_STENCIL_TEST );
		glStencilMask( 0xFF );
	}

	void StencilSystem::StencilWriterRenderImGui( Entity entity, StencilWriterComponent& stencilWriterC )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "StencilWriterComponent", &keep ) )
		{
			ImGuiHelper::StartPropertyTable();

			ImGuiHelper::RenderTableItem( "Write Value", [ & ]() { ImGui::DragScalar( "##Write Value", ImGuiDataType_U8, &stencilWriterC.WriteValue, 1 ); } );

			ImGuiHelper::EndPropertyTable();
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
