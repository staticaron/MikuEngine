#pragma once

#include <array>
#include <string>

#include "Core.h"

#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Vertex.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

namespace MikuEngine
{
	class MIKU_API Quad
	{
	public:
		Quad( const std::string& shaderPath = "" );
		~Quad() = default;

		void Init();

		static std::array<Vertex, 4> GetVerts();
		std::array<unsigned int, 6> GetIndices() const;

		const VertexBuffer& GetVB() const { return m_VB; }

		const VertexArray& GetVA() const { return m_VA; }

		const IndexBuffer& GetIB() const { return m_IB; }

		const Shader& GetShader() const { return m_Shader; }

	private:
		VertexBuffer m_VB;
		IndexBuffer m_IB;
		VertexBufferLayout m_VBL;
		VertexArray m_VA;

		std::string m_ShaderPath = RESOURCE_DIR "shaders/base.shader";
		Shader m_Shader;
	};
} // namespace MikuEngine
