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

		std::optional<SelectableItem> GetSelectedItem();
		void SetSelectedItem( UUID uuid, SelectableType type, AssetType assetType = AssetType::NONE );

		std::vector<Entity> GetAllEntities();
		std::optional<Entity> GetEntityByID( UUID id );
		std::optional<Entity> GetEntityFromEntt( entt::entity entity );
		std::optional<std::pair<const Entity, const CameraComponent&>> GetMainCamera() const;

		entt::registry& GetRegistry() { return m_Registry; }
		const entt::registry& GetRegistry() const { return m_Registry; }

		void Save();
		bool Load( const char* sceneFilePath = nullptr );
		void Clean();

	private:
		entt::registry m_Registry;
		SceneSerializer m_Serializer;

		std::optional<SelectableItem> m_SelectedItem;

		Material mat;

	protected:
		friend class SceneSerializer;
		friend class Entity;
	};
}
