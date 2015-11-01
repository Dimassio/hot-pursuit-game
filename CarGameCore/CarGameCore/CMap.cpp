#include "CMap.h"

Map::Map()
{
	size.first = 0;
	size.second = 0;
}

Map::Map( const Field& newMap ) :
	map( newMap )
{
	size.first = newMap[0].size();
	size.second = newMap.size();
}

Map::~Map()
{
}

void Map::print()
{
	for( size_t i = 0; i < size.second; ++i ) {
		for( size_t j = 0; j < size.first; ++j ) {
			std::cout << map[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Map::setPosition( int x, int y )
{
	map[y][x] = 5;
}

void Map::clearPosition( int x, int y )
{
	map[y][x] = 0;
}

Size Map::getSize()
{
	return this->size;
}

bool Map::isEmpty(int i, int j) {
	return ( map[j][i] != 1 );
}