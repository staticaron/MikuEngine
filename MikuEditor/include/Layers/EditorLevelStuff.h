#pragma once

namespace MikuEditor
{
	enum class PlayModeState
	{
		NOTPLAYING = 0,
		PLAYING,
		PAUSED,
	};

	class EditorLevelStuff
	{
	public:
		PlayModeState GetPlayModeState() const { return m_CurrentPlayModeState; }

	private:
		PlayModeState m_CurrentPlayModeState = PlayModeState::NOTPLAYING;

		friend class EditorLayer;
		friend class MenuBar;
	};
}
