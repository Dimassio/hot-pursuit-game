#pragma once

#include <string>
#include <vector>

const std::string RESOURCE_DIRECTORY = ".\\Resources\\";

namespace Core {
	enum Cell {
		ROAD = 0, FOREST = 1, CAR = 2
	};

	struct CCoordinates {
		CCoordinates( int newX, int newY ) :
			x( newX ),
			y( newY )
		{}

		bool operator == ( const CCoordinates &point ) const
		{
			return this->x == point.x && this->y == point.y;
		}

		CCoordinates operator + ( const CCoordinates &point ) const
		{
			return CCoordinates( this->x + point.x, this->y + point.y );
		}

		int x;
		int y;
	};

	typedef std::pair<CCoordinates, CCoordinates> CLine;
	typedef std::vector<std::vector<Cell>> CField;
}

typedef std::pair<size_t, size_t> CSize;