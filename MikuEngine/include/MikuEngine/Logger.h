#pragma once

#include <cassert>
#include <iostream>
#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"

namespace MikuEngine
{
	class MIKU_API Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef MIKU_DEBUG

#define MIKU_ASSERT( check, msg )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
	if ( !( check ) )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
		spdlog::error( msg );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  \
		assert( false );                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
	}

#define MIKU_CLIENT_INFO( ... )	 MikuEngine::Logger::GetClientLogger()->info( __VA_ARGS__ )
#define MIKU_CLIENT_WARN( ... )	 MikuEngine::Logger::GetClientLogger()->warn( __VA_ARGS__ )
#define MIKU_CLIENT_ERROR( ... ) MikuEngine::Logger::GetClientLogger()->error( __VA_ARGS__ )

#define MIKU_CORE_INFO( ... )  MikuEngine::Logger::GetCoreLogger()->info( __VA_ARGS__ )
#define MIKU_CORE_WARN( ... )  MikuEngine::Logger::GetCoreLogger()->warn( __VA_ARGS__ )
#define MIKU_CORE_ERROR( ... ) MikuEngine::Logger::GetCoreLogger()->error( __VA_ARGS__ );

#else

#define MIKU_CLIENT_INFO( ... )
#define MIKU_CLIENT_WARN( ... )
#define MIKU_CLIENT_ERROR( ... )

#define MIKU_CORE_INFO( ... )
#define MIKU_CORE_WARN( ... )
#define MIKU_CORE_ERROR( ... )

#endif
