#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace MikuEngine
{
	class UUID
	{
	public:
		UUID();
		UUID( uint64_t uuid );
		UUID( const std::string& uuidStr );

		std::string ToString() const { return std::to_string( m_UUID ); }

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template <>
	struct hash<MikuEngine::UUID>
	{
		std::size_t operator()( const MikuEngine::UUID& uuid ) const { return std::hash<uint64_t>()( ( uint64_t )uuid ); }
	};
}
