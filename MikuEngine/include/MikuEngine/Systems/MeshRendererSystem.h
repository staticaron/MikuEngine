#pragma once

#include <functional>

#include "entt/entity/fwd.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/node.h"

#include "Core.h"

namespace MikuEngine
{
	class Scene;
	class CameraData;
	class Entity;
	class MeshRendererComponent;
	class AppLevelStuff;
	enum class MaterialBlendMode;
	class MaterialContainer;
}

namespace MikuEngine
{
	struct MIKU_API DistancedEntity
	{
		entt::entity entt;
		const MeshRendererComponent* meshRendererC;
		const MaterialContainer* materialContainer;
		glm::mat4 transformMatrix;
		float distanceFromCamera;
	};

	class MIKU_API MeshRendererSystem
	{
	public:
		static void RenderMeshByType( const Scene& scene, AppLevelStuff& appLevelStuff, const CameraData& cameraData, MaterialBlendMode mode );

		static void MeshRendererComponentRenderImGui( Entity entity, MeshRendererComponent& meshRendererC, std::function<void()> modelEditBtnCallback, std::function<void()> materialEditBtnCallback );
		static void SerializeMeshRendererComponent( const Entity& entity, YAML::Emitter& emitter );
		static void DeSerializeMeshRendererComponent( MeshRendererComponent& meshRendererC, const YAML::Node& node );

	private:
		static void RenderMeshByBlendMode( const Scene& scene, AppLevelStuff& appLevelStuff, const std::vector<DistancedEntity>& distancedEntities, const CameraData& cameraData, const MaterialBlendMode& blendMode );
	};
}
