#pragma once

#include <functional>

#include "yaml-cpp/yaml.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class CameraData;
	class Entity;
	class MeshRendererComponent;
}

namespace MikuEngine
{
	class MIKU_API MeshRendererSystem
	{
	public:
		static void RenderMesh( const Scene& scene, const CameraData& cameraData );

		static void MeshRendererComponentRenderImGui( Entity entity, MeshRendererComponent& meshRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback );
		static void SerializeMeshRendererComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeMeshRendererComponent( MeshRendererComponent& meshRendererC, const YAML::Node& node );
	};
}
