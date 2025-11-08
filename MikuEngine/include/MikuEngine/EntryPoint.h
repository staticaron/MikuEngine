#pragma once

#include "Application.h"
#include <iostream>

extern MikuEngine::Application* MikuEngine::EntryPoint();

int main()
{
	std::cout << "Welcome to MikuEngine!!" << std::endl;

	MikuEngine::EntryPoint();
}
