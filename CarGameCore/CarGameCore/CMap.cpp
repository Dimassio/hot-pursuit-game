#include "CMap.h"

Map::Map()
{
}

Map::Map( const Field& newMap ) :
	map( newMap )
{
	size.first = 0; // todo: ���������������� �������� newMap
	size.second = 0;
}

Map::~Map()
{
}