#pragma once

#include "Core.h"
#include "Rendering/Primitives/Primitive.h"
#include "Rendering/Primitives/Vertex.h"

namespace MikuEngine
{
	class MIKU_API Mesh : public Primitive
	{
	public:
		Mesh( std::vector<Vertex3D> vertices, std::vector<unsigned int> indices );

		void Init() override;
		void SetupVertsAndIndices() override;

	public:
		std::vector<Vertex3D> m_Vertices;
		std::vector<unsigned int> m_Indices;
	};
}
