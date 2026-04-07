#pragma once

#include "glm/ext/matrix_float4x4.hpp"

#include "Core.h"

#include "Rendering/UniformBuffers/EditorUniformBuffer.h"
#include "Rendering/UniformBuffers/GameUniformBuffer.h"
#include "Rendering/UniformBuffers/LightingUniformBuffer.h"

namespace MikuEngine
{
	struct MatrixData
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		bool operator==( const MatrixData& matrixData ) const { return ( matrixData.projectionMatrix == projectionMatrix ) && ( matrixData.viewMatrix == viewMatrix ); }
	};

	struct LightingData
	{
		glm::vec3 position;
		glm::vec3 direction;
		float intensity;

		bool operator==( const LightingData& lightingData ) const { return ( lightingData.position == position ) && ( lightingData.direction == direction ) && ( lightingData.intensity == intensity ); }
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
