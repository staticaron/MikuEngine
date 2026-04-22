#pragma once

#include "Core.h"

#include "Scene/Scene.h"

namespace MikuEngine
{
	class MIKU_API SceneManager
	{
	public:
		Scene& GetScene();
		const Scene& GetScene() const;

		void StartPlayMode();
		void EndPlayMode();

	private:
		bool m_IsInPlayMode = false;

		Scene m_Scene;
		Scene m_PlaymodeScene;
	};
}
