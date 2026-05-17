#pragma once

#include "Application.h"
#include "Logger.h"

extern MikuEngine::Application* MikuEngine::EntryPoint();

int main()
{
	MikuEngine::Logger::Init();
	MIKU_CORE_DEBUG( "Welcome to MikuEngine!!" );

	MikuEngine::EntryPoint();
}
