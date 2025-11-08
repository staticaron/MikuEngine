#pragma once

#ifdef PLATFORM_WINDOWS
#ifdef BUILD_DLL
#define MIKU_API __declspec( dllexport )
#else
#define MIKU_API __declspec( dllimport )
#endif
#else
#define MIKU_API
#endif
