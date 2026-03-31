#include "Rendering/Primitives/Cube.h"

namespace MikuEngine
{
	void Cube::Init()
	{
		m_Model.LoadFromFile( RESOURCE_DIR "/models/Cube.fbx" );
	}

	void Cube::SetupVertsAndIndices() {}
}
