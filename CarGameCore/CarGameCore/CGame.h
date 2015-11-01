#pragma once

#include "CMap.h"
#include "CPlayer.h"
#include "CReader.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

struct PointsInformation {
	PointsInformation( const bool condition, const Coordinates& oldCoordinates, const Coordinates& newCoordinate ) :
		isAlive( condition ), previousCoordinates( oldCoordinates ), currentCoordinates( newCoordinate )
	{
	}

	bool isAlive;
	Coordinates previousCoordinates;
	Coordinates currentCoordinates;
};


class Game {
public:
	Game();

	Game( const Map& map, const PlayersInfo& playersInfo, const Line& startLine, const Reader& reader );

	~Game();

	void start();
	void finish( size_t winner );
	void fatalFinish();
	PointsInformation getPlayersBasePoints( size_t num ); // Отдаем Frontend у

private:
	Map map;
	Reader reader;
	size_t numOfDeadPlayers;
	std::vector<Player> players;
	Line startLine;

	int getPlayerOnFinish();
	bool startLineIntersectsWithPlayer( size_t num );
	void turnOfPlayer( size_t num );
	int playerCrashedIntoCar( size_t num );
	bool playerOutOfTrack( size_t num );
	void initPlayersPositionsInMap();
	void clearPlayersState( size_t num );
	void paintPlayersState( size_t num );
};