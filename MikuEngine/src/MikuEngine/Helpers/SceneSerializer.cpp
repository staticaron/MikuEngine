#include "Helpers/SceneSerializer.h"

#include "spdlog/spdlog.h"
#include "yaml-cpp/emitter.h"

namespace MikuEngine
{
	void SceneSerializer::Serialize( const Scene& scene )
	{
		YAML::Emitter emitter;
		emitter << "Something ";

		spdlog::info( emitter.c_str() );
	}
}
