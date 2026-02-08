#include "Helpers/SceneSerializer.h"

#include <cstdint>
#include <fstream>
#include <string_view>

#include "glm/fwd.hpp"
#include "nfd.h"
#include "yaml-cpp/yaml.h"

#include "Logger.h"

#include "Components.h"
#include "Entity.h"
#include "Scene/Scene.h"
#include "Systems.h"

namespace MikuEngine
{
	static void SerializeEntity( YAML::Emitter& emitter, Entity entity )
	{
		emitter << YAML::BeginMap;

		if ( entity.HasComponent<DataComponent>() )
		{
			auto data = entity.GetComponent<DataComponent>();
			emitter << YAML::Key << "name" << YAML::Value << data.EntityName;
		}

		if ( entity.HasComponent<IDComponent>() )
		{
			auto id = entity.GetComponent<IDComponent>();
			emitter << YAML::Key << "id" << YAML::Value << id.ID;
		}

		emitter << YAML::Key << "components" << YAML::Value << YAML::BeginSeq;

		if ( entity.HasComponent<TransformComponent>() )
		{
			TransformSystem::SerializeTransformComponent( entity, emitter );
		}

		if ( entity.HasComponent<SpriteRendererComponent>() )
		{
			RenderingSystem::SerializeSpriteRendererComponent( entity, emitter );
		}

		if ( entity.HasComponent<CameraComponent>() )
		{
			CameraSystem::SerializeCameraComponent( entity, emitter );
		}

		if ( entity.HasComponent<NativeScriptComponent>() )
		{
			ScriptExecutionSystem::SerializeNativeScriptComponent( entity, emitter );
		}

		emitter << YAML::EndSeq;

		emitter << YAML::EndMap;
	}

	void SceneSerializer::Serialize( Scene& scene, const std::string& savePath )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "scene" << YAML::Value << "Untitled";
		emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;

		for ( const auto& [ entity, idC ] : scene.m_Registry.view<IDComponent>().each() )
		{
			Entity entt( idC.ID, entity, &scene );
			SerializeEntity( emitter, entt );
		}

		emitter << YAML::EndMap;

		nfdchar_t* newPath = nullptr;

		nfdresult_t result = NFD_SaveDialog( "miku", nullptr, &newPath );

		if ( result != NFD_OKAY )
		{
			MIKU_CORE_ERROR( "Unable to Save to this Path!" );
			return;
		}

		std::ofstream fout( newPath );
		fout << emitter.c_str();
	}

	bool SceneSerializer::DeSerialize( Scene& scene, const std::string& loadPath )
	{
		nfdchar_t* newPath = nullptr;

		nfdresult_t result = NFD_OpenDialog( "miku", nullptr, &newPath );

		if ( result == NFD_CANCEL ) return false;

		if ( result != NFD_OKAY )
		{
			MIKU_CORE_ERROR( "Unable to load the scene file! File Not Found! " );
			return false;
		}

		YAML::Node node = YAML::LoadFile( newPath );

		MIKU_CORE_INFO( "Loading Scene : {}.miku", node[ "scene" ].as<std::string>() );

		YAML::Node entities = node[ "entities" ];

		if ( !entities.IsSequence() ) MIKU_CORE_ERROR( "Entities in Scene files are not in a sequence" );

		for ( const auto& entity : entities )
		{
			std::string name = entity[ "name" ].as<std::string>();
			UUID uuid = entity[ "id" ].as<uint64_t>();

			auto entt = scene.LoadEntity( name, uuid, &scene );

			MIKU_CORE_INFO( "Created Entity named : {} with ID : {}", name, std::to_string( uuid ) );

			const YAML::Node& components = entity[ "components" ];

			for ( const auto& component : components )
			{
				const std::string_view type = component[ "type" ].as<std::string_view>();
				YAML::Node values = component[ "values" ];

				if ( type == "TransformComponent" )
				{
					auto& transformC = entt.GetComponent<TransformComponent>();

					TransformSystem::DeSerializeTransformComponent( transformC, values );
				}

				if ( type == "SpriteRendererComponent" )
				{
					entt.AddComponent<SpriteRendererComponent>();
					auto& spriteRendererC = entt.GetComponent<SpriteRendererComponent>();

					RenderingSystem::DeSerializeSpriteRendererComponent( spriteRendererC, values );
				}

				if ( type == "CameraComponent" )
				{
					entt.AddComponent<CameraComponent>();
					auto& cameraC = entt.GetComponent<CameraComponent>();

					CameraSystem::DeSerializeCameraComponent( cameraC, values );
				}

				if ( type == "NativeScriptComponent" )
				{
					entt.AddComponent<NativeScriptComponent>();
					auto& nativeScriptC = entt.GetComponent<NativeScriptComponent>();

					ScriptExecutionSystem::DeSerializeNativeScriptComponent( nativeScriptC, values );
				}
			}
		}

		return true;
	}
}
