#pragma once

#include <functional>
#include <string>

namespace MikuEditor
{
	class BaseComponentInspector
	{
	protected:
		static void ComponentHeader( std::function<std::string()> getNameCallback, std::function<void( std::string )> setNameCallback );
		static void ComponentFooter( std::function<void()> deleteAssetCallback );
	};
}
