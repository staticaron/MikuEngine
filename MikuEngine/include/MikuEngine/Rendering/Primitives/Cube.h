#pragma once

#include "Core.h"
#include "Rendering/Model.h"
#include "Rendering/Primitives/Primitive.h"

namespace MikuEngine
{
	class Vertex;
}

namespace MikuEngine
{
	class MIKU_API Cube : public Primitive
	{
	public:
		void Init() override;
		void SetupVertsAndIndices() override;

	private:
		Model m_Model;
	};
}
