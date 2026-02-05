#include "MikuEngine/NativeScript.h"
#include "sample.h"

namespace Game
{
	extern "C"
	{
		MikuEngine::NativeScript* Function()
		{
			return new SampleClass();
		}
	}
}
