#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/wincolor_sink.h"

namespace MikuEngine
{
	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern( "%^[%T] %n: %v%$" );

		s_CoreLogger = spdlog::stdout_color_mt( "CORE" );
		s_CoreLogger->set_level( spdlog::level::trace );

		auto core_console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>( s_CoreLogger->sinks().back().get() );
		core_console_sink->set_color( spdlog::level::debug, core_console_sink->white );
		core_console_sink->set_color( spdlog::level::info, core_console_sink->green );
		core_console_sink->set_color( spdlog::level::warn, core_console_sink->yellow );
		core_console_sink->set_color( spdlog::level::err, core_console_sink->red );
		core_console_sink->set_color( spdlog::level::critical, core_console_sink->red_bold );

		s_ClientLogger = spdlog::stdout_color_mt( "EDITOR" );
		s_ClientLogger->set_level( spdlog::level::trace );

		auto client_console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>( s_CoreLogger->sinks().back().get() );
		client_console_sink->set_color( spdlog::level::debug, core_console_sink->white );
		client_console_sink->set_color( spdlog::level::info, core_console_sink->green );
		client_console_sink->set_color( spdlog::level::warn, core_console_sink->yellow );
		client_console_sink->set_color( spdlog::level::err, core_console_sink->red );
		client_console_sink->set_color( spdlog::level::critical, core_console_sink->red_bold );
	}
}
