#pragma once

#include "NativeScript.h"

namespace MikuEngine
{
	class MoveEntityScript : public NativeScript
	{
	public:
		MoveEntityScript() {}

		void OnReady( Entity entity ) override;
		void OnUpdate( Entity entity ) override;

	private:
	};
}
