#include "Systems/MeshRendererSystem.h"

#include "Application.h"
#include "Components.h"
#include "Data/CameraData.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	void MeshRendererSystem::RenderMesh( const Scene& scene, const CameraData& cameraData )
	{
		const auto& appLevelStuff = Application::GetAppLevelStuff();
		const auto& renderer = appLevelStuff.GetRenderer();
		const auto& cube = renderer.GetCube();

		auto entities = scene.GetRegistry().view<DataComponent, MeshRendererComponent>();

		for ( const auto& [ entity, dataC, meshRendererC ] : entities.each() )
		{
			const auto& transformC = scene.GetRegistry().get<TransformComponent>( entity );

			auto mvp = transformC.GetModelMatrix();

			const auto& shader = appLevelStuff.GetShaderManager().GetDefaultShader().shader;

			renderer.Draw( cube.GetVA(), cube.GetIB(), shader );
		}
	}
}
