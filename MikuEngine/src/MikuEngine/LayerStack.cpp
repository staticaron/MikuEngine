#include "LayerStack.h"
#include "Layer.h"

namespace MikuEngine
{
	void LayerStack::Push( Layer layer )
	{
		m_Layers.push_back( layer );
	}
}
