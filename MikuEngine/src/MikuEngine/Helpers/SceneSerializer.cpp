#include "Helpers/SceneSerializer.h"

#include <cstdint>
#include <fstream>
#include <string_view>

#include "nfd.h"
#include "yaml-cpp/emitter.h"

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

		if ( entity.HasComponent<IDComponent>() )
		{
			auto id = entity.GetComponent<IDComponent>();
			emitter << YAML::Key << "id" << YAML::Value << id.ID;
		}

		if ( entity.HasComponent<DataComponent>() )
		{
			auto data = entity.GetComponent<DataComponent>();
			emitter << YAML::Key << "name" << YAML::Value << data.EntityName;
			auto parentUUID = data.ParentUUID;
			emitter << YAML::Key << "parent" << YAML::Value << ( parentUUID.has_value() ? parentUUID->ToString() : "0" );
		}

		emitter << YAML::Key << "components" << YAML::Value << YAML::BeginSeq;

		if ( entity.HasComponent<TransformComponent>() ) TransformSystem::SerializeTransformComponent( entity, emitter );
		if ( entity.HasComponent<SpriteRendererComponent>() ) SpriteRendererSystem::SerializeSpriteRendererComponent( entity, emitter );
		if ( entity.HasComponent<MeshRendererComponent>() ) MeshRendererSystem::SerializeMeshRendererComponent( entity, emitter );
		if ( entity.HasComponent<CameraComponent>() ) CameraSystem::SerializeCameraComponent( entity, emitter );
		if ( entity.HasComponent<NativeScriptComponent>() ) ScriptExecutionSystem::SerializeNativeScriptComponent( entity, emitter );
		if ( entity.HasComponent<DirectionalLightComponent>() ) LightingSystem::SerializeDirectionalLightComponent( entity, emitter );
		if ( entity.HasComponent<SkyboxComponent>() ) SkyboxRendererSystem::SerializeSkyboxComponent( entity, emitter );
		if ( entity.HasComponent<StencilReaderComponent>() ) StencilSystem::SerializeStencilReader( entity, emitter );
		if ( entity.HasComponent<StencilWriterComponent>() ) StencilSystem::SerializeStencilWriter( entity, emitter );

		emitter << YAML::EndSeq;

		emitter << YAML::EndMap;
	}

	void SceneSerializer::Serialize( Scene& scene )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "scene" << YAML::Value << "Untitled";
		emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;

		for ( const auto& [ entity, idC, dataC ] : scene.m_Registry.view<IDComponent, DataComponent>().each() )
		{
			Entity entt( idC.ID, entity, &scene, dataC.EntityName, dataC.ParentUUID );
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

	bool SceneSerializer::DeSerialize( Scene& scene, const char* sceneFilePath )
	{
		nfdchar_t* loadPath = ( nfdchar_t* )sceneFilePath;

		if ( sceneFilePath == nullptr )
		{
			nfdresult_t result = NFD_OpenDialog( "miku", nullptr, &loadPath );

			MIKU_CORE_INFO( "Scene Load Path: {}", loadPath );

			if ( result == NFD_CANCEL ) return false;

			if ( result != NFD_OKAY )
			{
				MIKU_CORE_ERROR( "Unable to load the scene file! File Not Found! " );
				return false;
			}
		}

		YAML::Node node = YAML::LoadFile( loadPath );

		YAML::Node entities = node[ "entities" ];

		if ( !entities.IsSequence() ) MIKU_CORE_ERROR( "Entities in Scene files are not in a sequence" );

		for ( const auto& entity : entities )
		{
			std::string name = entity[ "name" ].as<std::string>();
			UUID uuid = entity[ "id" ].as<uint64_t>();

			std::optional<UUID> parentUUID;
			parentUUID = entity[ "parent" ].as<uint64_t>();

			if ( parentUUID == 0 ) parentUUID = std::nullopt;

			auto entt = scene.LoadEntity( name, uuid, &scene, parentUUID );

			MIKU_CORE_DEBUG( "Created Entity named : {} with ID : {}", name, std::to_string( uuid ) );

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

					SpriteRendererSystem::DeSerializeSpriteRendererComponent( spriteRendererC, values );
				}

				if ( type == "MeshRendererComponent" )
				{
					entt.AddComponent<MeshRendererComponent>();
					auto& meshRendererC = entt.GetComponent<MeshRendererComponent>();

					MeshRendererSystem::DeSerializeMeshRendererComponent( meshRendererC, values );
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

				if ( type == "DirectionalLightComponent" )
				{
					entt.AddComponent<DirectionalLightComponent>();
					auto& directionalLightC = entt.GetComponent<DirectionalLightComponent>();

					LightingSystem::DeSerializeDirectionalLightComponent( directionalLightC, values );
				}

				if ( type == "SkyboxComponent" )
				{
					entt.AddComponent<SkyboxComponent>();
					auto& skyboxC = entt.GetComponent<SkyboxComponent>();

					SkyboxRendererSystem::DeSerializeSkyboxComponent( skyboxC, values );
				}

				if ( type == "StencilReaderComponent" )
				{
					entt.AddComponent<StencilReaderComponent>();
					auto& stencilReaderC = entt.GetComponent<StencilReaderComponent>();

					StencilSystem::DeSerializeStencilReader( stencilReaderC, values );
				}

				if ( type == "StencilWriterComponent" )
				{
					entt.AddComponent<StencilWriterComponent>();
					auto& stencilWriterC = entt.GetComponent<StencilWriterComponent>();

					StencilSystem::DeSerializeStencilWriter( stencilWriterC, values );
				}
			}
		}

		return true;
	}
}
