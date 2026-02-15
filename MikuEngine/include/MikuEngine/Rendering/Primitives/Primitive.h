#pragma once

#include "Core.h"

#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	class MIKU_API Primitive
	{
	public:
		virtual void Init() = 0;
		virtual void SetupVertsAndIndices() = 0;

		const VertexBuffer& GetVB() const { return m_VB; }
		const VertexArray& GetVA() const { return m_VA; }
		const IndexBuffer& GetIB() const { return m_IB; }

	protected:
		VertexBuffer m_VB;
		IndexBuffer m_IB;
		VertexBufferLayout m_VBL;
		VertexArray m_VA;
	};
}
