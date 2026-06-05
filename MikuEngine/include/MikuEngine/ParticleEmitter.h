#pragma once

#include "Core.h"
#include "Data/ParticleEmitterProperties.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Primitives/Vertex.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "UUID.h"

namespace MikuEngine
{
	struct MIKU_API Particle
	{
		bool IsParticleVisible = false;
		glm::vec2 ParticleVelocity = { 1.0f, 1.0f };
	};

	class MIKU_API ParticleEmitter
	{
	public:
		void Init( const ParticleEmitterProperties& properties );

	private:
		ParticleEmitterProperties m_Properties;

		std::vector<Vertex> m_ParticleVertices;
		std::vector<unsigned int> m_ParticleIndices;
		std::vector<Particle> m_Particles;

		VertexBuffer m_VB;
		IndexBuffer m_IB;
		VertexBufferLayout m_VBL;
		VertexArray m_VA;
	};
}
