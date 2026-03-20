#pragma once

#include "glm/glm.hpp"

#include "Core.h"

#include "Rendering/EditorUniformBuffer.h"
#include "Rendering/GameUniformBuffer.h"

namespace MikuEngine
{
	struct MatrixData
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		bool operator==( const MatrixData& matrixData ) const { return ( matrixData.projectionMatrix == projectionMatrix ) && ( matrixData.viewMatrix == viewMatrix ); }
	};

	class MIKU_API UniformBufferManager
	{
	public:
		void Init();

		const UniformBuffer& GetGameUniformBuffer() const { return m_GameUniformBuffer; }
		const UniformBuffer& GetEditorUniformBuffer() const { return m_EditorUniformBuffer; }

		void UpdateBuffer();

		void UpdateGameMatrixData( const MatrixData& matrixData );
		void UpdateEditorMatrixData( const MatrixData& matrixData );

	private:
		GameUniformBuffer m_GameUniformBuffer;
		EditorUniformBuffer m_EditorUniformBuffer;

		MatrixData m_GameMatrixData;
		MatrixData m_EditorMatrixData;
	};
}
