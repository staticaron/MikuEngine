#pragma once

#include "spdlog/spdlog.h"

namespace MikuEngine
{
#ifdef MIKU_DEBUG
#define MIKU_INFO( ... )  spdlog::info( __VA_ARGS__ );
#define MIKU_WARN( ... )  spdlog::warn( __VA_ARGS__ );
#define MIKU_ERROR( ... ) spdlog::error( __VA_ARGS__ );
#else
#define MIKU_INFO( ... )
#define MIKU_WARNING( ... )
#define MIKU_ERROR( ... )
#define MIKU_INFO( ... )
#endif
}
