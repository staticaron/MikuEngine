#pragma once

#include <optional>
#include <vector>

#include "entt/entt.hpp"

#include "Core.h"
#include "Data/SelectableItem.h"
#include "Helpers/SceneSerializer.h"
#include "Rendering/Material.h"
#include "UUID.h"

namespace MikuEngine
{
	class AppLevelStuff;
	class Entity;
	class CameraComponent;
	class DirectionalLightComponent;
	class CameraData;
}

namespace MikuEngine
{
	class MIKU_API Scene
	{
	public:
		Scene();
		~Scene() = default;

		void Update( double dt );
		void Render( AppLevelStuff& appLevelStuff ) const;
		void RenderInEditor( AppLevelStuff& appLevelStuff, const CameraData& cameraData ) const;
		void RenderImGui( const AppLevelStuff& appLevelStuff );

		Entity CreateEntity( const std::string& name, Scene* parentScene );
		Entity LoadEntity( const std::string& name, UUID uuid, Scene* parentScene );
		void AddEntityToDeleteQueue( const UUID& uuid ) { m_DeleteQueue.push_back( uuid ); }

		void PerformDeletions();

		std::optional<SelectableItem> GetSelectedItem();
		void SetSelectedItem( UUID uuid, SelectableType type, AssetType assetType = AssetType::NONE );

		std::vector<Entity> GetAllEntities();
		std::optional<Entity> GetEntityByID( UUID id );
		std::optional<Entity> GetEntityFromEntt( entt::entity entity );

		std::optional<std::pair<const Entity, const CameraComponent&>> GetMainCamera() const;
		std::optional<std::pair<const Entity, const DirectionalLightComponent&>> GetMainLight() const;

		entt::registry& GetRegistry() { return m_Registry; }
		const entt::registry& GetRegistry() const { return m_Registry; }

		void Save();
		bool Load( const char* sceneFilePath = nullptr );
		void Clean();

	private:
		void DeleteEntity( const UUID& uuid );

	private:
		entt::registry m_Registry;
		SceneSerializer m_Serializer;

		std::optional<SelectableItem> m_SelectedItem;

		std::vector<UUID> m_DeleteQueue;

		Material mat;

	protected:
		friend class SceneSerializer;
		friend class Entity;
	};
}
