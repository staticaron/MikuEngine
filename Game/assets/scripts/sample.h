#pragma once

#include "MikuEngine/Helpers/DLLloader.h"
#include "MikuEngine/NativeScript.h"

namespace Game
{
	class SampleClass : public MikuEngine::NativeScript
	{
	public:
		void OnUpdate( MikuEngine::Entity entity ) override;

	private:
	};
}

REGISTER_SCRIPT( Game::SampleClass, "SampleClass" );
