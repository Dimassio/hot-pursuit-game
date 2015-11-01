#pragma once

#include <string>

namespace Core {
	class CMap;

	class CReader {
	public:
		CMap ReadMap( const std::string& path );
	};
}