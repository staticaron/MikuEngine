#pragma once

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Entity;
	class StencilReaderComponent;
	class StencilWriterComponent;
}

namespace MikuEngine
{
	class MIKU_API StencilSystem
	{
	public:
		static void StencilReaderRenderImGui( Entity entity, StencilReaderComponent& spriteRendererC );
		static void SerializeStencilReader( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeStencilReader( StencilReaderComponent& spriteRendererC, const YAML::Node& node );

		static void StencilWriterRenderImGui( Entity entity, StencilWriterComponent& spriteRendererC );
		static void SerializeStencilWriter( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeStencilWriter( StencilWriterComponent& spriteRendererC, const YAML::Node& node );

	private:
	};
}
