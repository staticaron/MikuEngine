#pragma once

#include "Core.h"
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

		static std::array<Vertex, 24> GetVerts();
		static std::array<unsigned int, 36> GetIndices();

	private:
		static std::array<Vertex, 24> m_Verts;
		static std::array<unsigned int, 36> m_Indices;
	};
}
