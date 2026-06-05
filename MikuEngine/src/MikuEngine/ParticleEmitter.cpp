#include "ParticleEmitter.h"

#include "Logger.h"
#include "Rendering/Primitives/Vertex.h"

namespace MikuEngine
{
	void ParticleEmitter::Init( const ParticleEmitterProperties& properties )
	{
		m_VB.Init( properties.MaxParticleCount * 4 * sizeof( Vertex ), nullptr );
		m_IB.Init( properties.MaxParticleCount * 6, nullptr );

		m_ParticleVertices.reserve( properties.MaxParticleCount * 4 );
		m_ParticleIndices.reserve( properties.MaxParticleCount * 6 );

		m_Particles.reserve( properties.MaxParticleCount );

		for ( size_t x = 0; x < properties.MaxParticleCount; x++ )
		{
			// Insert the Verts
			// clang-format off
			m_ParticleVertices.push_back( {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }} );
			m_ParticleVertices.push_back( { { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }} );
			m_ParticleVertices.push_back( { { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f } } );
			m_ParticleVertices.push_back( {{ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f } } );
			// clang-format on

			// Insert the Indices
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 0 );
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 1 );
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 2 );
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 2 );
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 3 );
			m_ParticleIndices.push_back( ( 3 + 1 ) * x + 0 );

			// Insert the particle details
			m_Particles.push_back( {} );
		}

		size_t bufferSize = sizeof( m_ParticleVertices[ 0 ] ) * m_ParticleVertices.size();
		size_t bufferSizeRaw = sizeof( Vertex ) * m_ParticleVertices.size();

		m_VB.PutData( m_ParticleVertices.data(), sizeof( m_ParticleVertices[ 0 ] ) * m_ParticleVertices.size() );
		m_IB.PutData( m_ParticleIndices.data(), m_ParticleVertices.size() );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Init();
		m_VA.Setup( m_VB, m_VBL );

		MIKU_CORE_INFO( "Particle Emitter Created with Particle Count : {}", properties.MaxParticleCount );
	}
}
