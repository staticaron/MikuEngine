#include "Managers/AssetLoader.h"

#include "Application.h"
#include "Logger.h"

#include <filesystem>
#include <unistd.h>

namespace MikuEngine
{
	void AssetLoader::Init()
	{
		m_Fd = inotify_init1( IN_NONBLOCK );

		// Add the Project's Asset Directory
		std::string projectDir = PROJECT_DIR;
		WatchDirectory( projectDir );

		// Add the project's subdirectories
		for ( auto item : std::filesystem::recursive_directory_iterator( PROJECT_DIR ) )
		{
			if ( item.is_directory() == false )
				continue;
			WatchDirectory( item.path() );
		}
	}

	void AssetLoader::Update()
	{
		m_CreateEvents.clear();
		m_MovedFromEvents.clear();
		m_MovedToEvents.clear();
		m_DeleteEvents.clear();
		m_ModifyEvents.clear();

		std::vector<char> buffer( 4096 );

		int rd = read( m_Fd, buffer.data(), buffer.size() );
		if ( rd < 0 )
			return;

		inotify_event* events = ( inotify_event* )buffer.data();

		unsigned int readBytes{ 0 };

		// Gather all the events
		while ( readBytes < rd )
		{
			inotify_event* event = ( inotify_event* )( buffer.data() + readBytes );

			if ( event->mask & IN_CREATE )
				m_CreateEvents.push_back( event );
			if ( event->mask & IN_MODIFY )
				m_ModifyEvents.push_back( event );
			if ( event->mask & IN_MOVED_FROM )
				m_MovedFromEvents.push_back( event );
			if ( event->mask & IN_MOVED_TO )
				m_MovedToEvents.push_back( event );
			if ( event->mask & IN_DELETE )
				m_DeleteEvents.push_back( event );

			readBytes += sizeof( inotify_event ) + event->len;
		}

		// Process all the events
		for ( auto event : m_DeleteEvents )
		{
			std::string fileParentFolder = m_WdList[ event->wd ] + "/";
			auto filepath = fileParentFolder + event->name;

			if ( filepath.ends_with( ".meta" ) == false )
			{
				if ( event->mask & IN_CREATE )
				{
					if ( event->mask & IN_ISDIR )
					{
						MIKU_CORE_DEBUG( "Directory Created @ {}", filepath.c_str() );
						WatchDirectory( filepath );
					}
				}

				if ( event->mask & IN_MOVED_FROM )
				{
					Application::GetAppLevelStuff().GetAssetPoolManager().RemoveAssetOfFile( filepath );
					MIKU_CORE_INFO( "Move File From @ {}", filepath.c_str() );
				}

				if ( event->mask & IN_MOVED_TO )
				{
					Application::GetAppLevelStuff().GetAssetPoolManager().LoadAssetFromFile( filepath );
					MIKU_CORE_INFO( "Move File To @ {}", filepath.c_str() );
				}

				if ( event->mask & IN_DELETE )
				{
				}

				if ( event->mask & IN_MODIFY )
				{
					Application::GetAppLevelStuff().GetAssetPoolManager().RemoveAssetOfFile( filepath );
					Application::GetAppLevelStuff().GetAssetPoolManager().LoadAssetFromFile( filepath );
					MIKU_CORE_INFO( "Modify File @ {}", filepath.c_str() );
				}
			}
		}

		for ( auto fromEvent : m_MovedFromEvents )
		{
			std::string fileParentFolder = m_WdList[ fromEvent->wd ] + "/";
			auto filepath = fileParentFolder + fromEvent->name;

			for ( auto toEvent : m_MovedToEvents )
			{
				if ( fromEvent->cookie == toEvent->cookie )
				{
					// RENAME
					// Application::GetAppLevelStuff().GetAssetPoolManager().RenameAssetFile( filepath, toEvent->name );

					MIKU_CORE_DEBUG( "Filed Renamed from {} to {}", fromEvent->name, toEvent->name );
					return;
				}
			}

			MIKU_CORE_DEBUG( "File Moved From {}" );
		}

		// Handle all the delete events
		for ( auto event : m_DeleteEvents )
		{
			std::string fileParentFolder = m_WdList[ event->wd ] + "/";
			auto filepath = fileParentFolder + event->name;

			Application::GetAppLevelStuff().GetAssetPoolManager().RemoveAssetOfFile( filepath );
			MIKU_CORE_INFO( "Delete File @ {}", filepath.c_str() );
		}
	}

	void AssetLoader::Destroy()
	{
		for ( auto [ wd, path ] : m_WdList )
			UnWatchDirectory( path, wd );

		close( m_Fd );
	}

	void AssetLoader::WatchDirectory( const std::string& directoryPath )
	{
		m_WdList[ inotify_add_watch( m_Fd, directoryPath.c_str(), m_WatchMasks ) ] = directoryPath;
		MIKU_CORE_INFO( "[ADD WATCH] {}", directoryPath.c_str() );
	}

	void AssetLoader::UnWatchDirectory( const std::string& directoryPath, int wd )
	{
		inotify_rm_watch( m_Fd, wd );
		MIKU_CORE_DEBUG( "[REMOVE WATCH] {}", directoryPath );
	}

}
