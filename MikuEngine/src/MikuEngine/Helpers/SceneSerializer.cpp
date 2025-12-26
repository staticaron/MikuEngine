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
			emitter << YAML::BeginMap;
			auto transform = entity.GetComponent<TransformComponent>();
			emitter << YAML::Key << "type" << YAML::Value << "TransformComponent";

			emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
			emitter << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Position.x << transform.Position.y << transform.Position.z << YAML::EndSeq;
			emitter << YAML::Key << "rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Rotation.x << transform.Rotation.y << transform.Rotation.z << YAML::EndSeq;
			emitter << YAML::Key << "scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << transform.Scale.x << transform.Scale.y << transform.Scale.z << YAML::EndSeq;
			emitter << YAML::EndMap;

			emitter << YAML::EndMap;
		}

		if ( entity.HasComponent<SpriteRendererComponent>() )
		{
			emitter << YAML::BeginMap;

			auto spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
			emitter << YAML::Key << "type" << YAML::Value << "SpriteRendererComponent";

			emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
			emitter << YAML::Key << "texture" << YAML::Value << spriteRenderer.TextureIdentifier;
			emitter << YAML::Key << "tint" << YAML::Value << YAML::Flow << YAML::BeginSeq << spriteRenderer.Tint.x << spriteRenderer.Tint.y << spriteRenderer.Tint.z << spriteRenderer.Tint.w << YAML::EndSeq;
			emitter << YAML::EndMap;

			emitter << YAML::EndMap;
		}

		if ( entity.HasComponent<CameraComponent>() )
		{
			emitter << YAML::BeginMap;

			auto cameraComponent = entity.GetComponent<CameraComponent>();
			emitter << YAML::Key << "type" << YAML::Value << "CameraComponent";

			emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
			emitter << YAML::Key << "zoom" << YAML::Value << cameraComponent.Zoom;
			emitter << YAML::EndMap;

			emitter << YAML::EndMap;
		}

		emitter << YAML::EndSeq;

		emitter << YAML::EndMap;
	}

	static bool DecodeVec3( const YAML::Node& node, glm::vec3& container )
	{
		if ( !node.IsSequence() || node.size() != 3 ) return false;

		container.x = node[ 0 ].as<float>();
		container.y = node[ 1 ].as<float>();
		container.z = node[ 2 ].as<float>();

		return true;
	}

	static bool DecodeVec4( const YAML::Node& node, glm::vec4& container )
	{
		if ( !node.IsSequence() || node.size() != 4 ) return false;

		container.x = node[ 0 ].as<float>();
		container.y = node[ 1 ].as<float>();
		container.z = node[ 2 ].as<float>();
		container.w = node[ 3 ].as<float>();

		return true;
	}

	void SceneSerializer::Serialize( Scene& scene, const std::string& savePath )
	{
		YAML::Emitter emitter;

		emitter << YAML::BeginMap;
		emitter << YAML::Key << "scene" << YAML::Value << "Untitled";
		emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;

		for ( auto entity : scene.m_Registry.view<entt::entity>() )
		{
			Entity entt( entity, &scene );
			SerializeEntity( emitter, entt );
		}

		emitter << YAML::EndMap;

		nfdchar_t* newPath = nullptr;

		nfdresult_t result = NFD_SaveDialog( "miku", nullptr, &newPath );

		if ( result != NFD_OKAY )
		{
			MIKU_ERROR( "Unable to Save to this Path!" );
			return;
		}

		std::ofstream fout( newPath );
		fout << emitter.c_str();
	}

	bool SceneSerializer::DeSerialize( Scene& scene, const std::string& loadPath )
	{
		nfdchar_t* newPath = nullptr;

		nfdresult_t result = NFD_OpenDialog( "miku", nullptr, &newPath );

		if ( result != NFD_OKAY )
		{
			MIKU_ERROR( "Unable to load the scene file! File Not Found! " )
			return false;
		}

		YAML::Node node = YAML::LoadFile( newPath );

		MIKU_INFO( "Loading Scene : ", node[ "scene" ].as<std::string>() );

		YAML::Node entities = node[ "entities" ];

		if ( !entities.IsSequence() ) MIKU_ERROR( "Entities in Scene files are not in a sequence" );

		for ( const auto& entity : entities )
		{
			std::string name = entity[ "name" ].as<std::string>();
			UUID uuid = entity[ "id" ].as<uint64_t>();

			auto entt = scene.LoadEntity( name, uuid, &scene );

			MIKU_INFO( "Created Entity named : {}", name );

			const YAML::Node& components = entity[ "components" ];

			for ( const auto& component : components )
			{
				const std::string_view type = component[ "type" ].as<std::string_view>();
				YAML::Node values = component[ "values" ];

				if ( type == "TransformComponent" )
				{
					glm::vec3 position;
					glm::vec3 rotation;
					glm::vec3 scale;

					DecodeVec3( values[ "position" ], position );
					DecodeVec3( values[ "rotation" ], rotation );
					DecodeVec3( values[ "scale" ], scale );

					entt.GetComponent<TransformComponent>().Position = position;
					entt.GetComponent<TransformComponent>().Rotation = rotation;
					entt.GetComponent<TransformComponent>().Scale = scale;
				}

				if ( type == "SpriteRendererComponent" )
				{
					std::string_view texture = values[ "texture" ].as<std::string_view>();

					glm::vec4 tint;
					DecodeVec4( values[ "tint" ], tint );

					entt.AddComponent<SpriteRendererComponent>();
					entt.GetComponent<SpriteRendererComponent>().TextureIdentifier = texture;
					entt.GetComponent<SpriteRendererComponent>().Tint = tint;
				}

				if ( type == "CameraComponent" )
				{
					float zoom = values[ "zoom" ].as<float>();

					entt.AddComponent<CameraComponent>();
					entt.GetComponent<CameraComponent>().Zoom = zoom;
				}

				MIKU_INFO( "Created {} for {}", type, name );
			}
		}

		return true;
	}
}
