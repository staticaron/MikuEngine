#pragma once

#include "Core.h"

#include <vector>

#include "Layer.h"

namespace MikuEngine
{
	class MIKU_API LayerStack
	{
	public:
		void Push( Layer layer );

		Layer& GetLayer( unsigned int index )
		{
			return m_Layers[ index ];
		}

		unsigned int GetCount()
		{
			return m_Layers.size();
		}

	private:
		std::vector<Layer> m_Layers;
	};
}
