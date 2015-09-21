#pragma once

#include <vector>

typedef std::vector< std::vector<size_t> > Field;

class Map {
public:
	Map();

	Map( const Field& newMap );

	~Map();

private:
	Field map; // �������, �������� ������ 1/0. 1 - �����������, 0 - ��������� ������
};