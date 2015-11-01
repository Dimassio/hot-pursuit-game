#pragma once

#include <fstream>
#include <iostream>
#include "CPlayer.h"
#include <exception>
#include <string>
#include <assert.h>

typedef std::vector< std::vector<size_t> > Field;

struct Line {
	Line()
	{
	}

	Line( Coordinates x, Coordinates y ) :
		firstPoint( x ), secondPoint( y )
	{
	}

	Coordinates firstPoint;
	Coordinates secondPoint;
};

class Reader {

public:
	Reader();

	~Reader();

	Field readMap( const std::string& path );

	PlayersInfo readPlayers();

	int readPlayersChoice( size_t num );

	Line readLine();
};