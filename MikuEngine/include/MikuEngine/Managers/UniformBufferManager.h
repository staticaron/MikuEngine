#pragma once

#include "Core.h"

#include "Data/UniformBuffer/LightingData.h"
#include "Data/UniformBuffer/MatrixData.h"

#include "Rendering/UniformBuffers/EditorUniformBuffer.h"
#include "Rendering/UniformBuffers/GameUniformBuffer.h"
#include "Rendering/UniformBuffers/LightingUniformBuffer.h"

namespace MikuEngine
{
	class MIKU_API UniformBufferManager
	{
	public:
		void Init();

		const UniformBuffer& GetGameUniformBuffer() const { return m_GameUniformBuffer; }
		const UniformBuffer& GetEditorUniformBuffer() const { return m_EditorUniformBuffer; }
		const UniformBuffer& GetLightingUniformBuffer() const { return m_LightingUniformBuffer; }

		void UpdateBuffer();

		void UpdateGameMatrixData( const MatrixData& matrixData );
		void UpdateEditorMatrixData( const MatrixData& matrixData );
		void UpdateLightingData( const LightingData& lightingData );

	private:
		GameUniformBuffer m_GameUniformBuffer;
		EditorUniformBuffer m_EditorUniformBuffer;
		LightingUniformBuffer m_LightingUniformBuffer;

		MatrixData m_GameMatrixData;
		MatrixData m_EditorMatrixData;

		LightingData m_LightingData;
	};
}
