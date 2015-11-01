#pragma once

#include <vector>
#include <iostream>

typedef std::pair<size_t, size_t> Size;
typedef std::vector< std::vector<size_t> > Field;

class Map {
public:
	Map();

	Map( const Field& newMap );

	~Map();

	void print();
	void setPosition( int x, int y );
	void clearPosition( int x, int y );
	Size getSize();
	bool isEmpty( int i, int j );

private:
	Field map; // Карта (таблица), в которой 1 - препятствие (дороги нет). А 0 - дорога.
	Size size;
};