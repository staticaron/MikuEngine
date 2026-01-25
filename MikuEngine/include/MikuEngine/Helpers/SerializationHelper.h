#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

namespace MikuEngine
{
	static bool DecodeVec2( const YAML::Node& node, glm::vec2& container )
	{
		if ( !node.IsSequence() || node.size() != 2 ) return false;

		container.x = node[ 0 ].as<float>();
		container.y = node[ 1 ].as<float>();

		return true;
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
}
