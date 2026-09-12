#pragma once

#include <cstdint>
#include <string>
#include <sys/inotify.h>
#include <unordered_map>
#include <vector>

namespace MikuEngine
{
	class AssetLoader
	{
	public:
		void Init();
		void Update();
		void Destroy();

		void WatchDirectory( const std::string& directoryPath );
		void UnWatchDirectory( const std::string& directoryPath, int wd );

	private:
		int m_Fd{};
		std::unordered_map<int, std::string> m_WdList{};

		uint32_t m_WatchMasks = IN_ALL_EVENTS;

		std::vector<inotify_event*> m_CreateEvents;
		std::vector<inotify_event*> m_MovedFromEvents;
		std::vector<inotify_event*> m_MovedToEvents;
		std::vector<inotify_event*> m_DeleteEvents;
		std::vector<inotify_event*> m_ModifyEvents;
	};
}
