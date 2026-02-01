#pragma once

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API NativeScript
	{
	public:
		virtual void OnCreate() {}
		virtual void OnReady() {}
		virtual void OnUpdate() {}

	private:
	};
}
