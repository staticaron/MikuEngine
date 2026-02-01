#pragma once

#include "ScriptableEntity.h"

namespace MikuEngine
{
	class MoveEntityScript : public ScriptableEntity
	{
	public:
		MoveEntityScript( Entity entity ) : ScriptableEntity( entity ) {}

		void OnReady() override;
		void OnUpdate() override;

	private:
	};
}
