#include "Rendering/Managers/UniformBufferManager.h"

#include "MikuEngine/Logger.h"

namespace MikuEngine
{
	void UniformBufferManager::Init()
	{
		m_GameUniformBuffer.Init( sizeof( MatrixData ) );
		m_EditorUniformBuffer.Init( sizeof( MatrixData ) );
	}

	void UniformBufferManager::UpdateGameMatrixData( const MatrixData& matrixData )
	{
		bool result = matrixData == m_GameMatrixData;
		if ( result ) return;

		m_GameMatrixData = matrixData;
		m_GameUniformBuffer.PutData( &m_GameMatrixData, sizeof( MatrixData ) );
	}

	void UniformBufferManager::UpdateEditorMatrixData( const MatrixData& matrixData )
	{
		if ( matrixData == m_EditorMatrixData ) return;

		m_EditorMatrixData = matrixData;
		m_EditorUniformBuffer.PutData( &m_EditorMatrixData, sizeof( MatrixData ) );
	}
}
