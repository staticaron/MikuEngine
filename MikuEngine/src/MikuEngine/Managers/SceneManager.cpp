#include "Managers/SceneManager.h"

namespace MikuEngine
{
	Scene& SceneManager::GetScene()
	{
		if ( m_IsInPlayMode ) return m_PlaymodeScene;
		return m_Scene;
	}

	const Scene& SceneManager::GetScene() const
	{
		if ( m_IsInPlayMode ) return m_PlaymodeScene;
		return m_Scene;
	}

	void SceneManager::StartPlayMode()
	{
		m_IsInPlayMode = true;
		m_PlaymodeScene.CopyFrom( m_Scene );
	}

	void SceneManager::EndPlayMode()
	{
		m_IsInPlayMode = false;
	}
}
