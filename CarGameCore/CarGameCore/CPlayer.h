#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

typedef std::pair<size_t, size_t> Size;

struct Coordinates {
	Coordinates( int newX, int newY ) :
		x( newX ), y( newY )
	{
	}

	Coordinates()
	{
	}

	bool operator == ( const Coordinates &point ) const
	{
		return this->x == point.x && this->y == point.y;
	}

	Coordinates operator + ( const Coordinates &point ) const
	{
		return Coordinates( this->x + point.x, this->y + point.y );
	}

	int x;
	int y;
};

enum Directions {
	SW = 1, S, SE,
	W, C, E, // `C` for no direction
	NW, N, NE
};

struct PlayersInfo {
	PlayersInfo()
	{
		numberOfPlayers = 0;
	}

	size_t numberOfPlayers;
	std::vector<Coordinates> positions;
};

class Player {
public:
	Player();
	Player( const Coordinates& coordinates, const bool isAlive );
	~Player();

	void die( void );
	void goToStart( void );

	Coordinates getPosition( void );
	void move( int, Size );
	Coordinates getPreviousPosition( void );
	bool wasFirstStep();
	bool wasSecondStep();
	void makeFirstStep();
	void makeSecondStep();
	bool directionIsValid( int direction, const Size& size );
	bool playerIsAlive();

private:
	Coordinates position;
	Coordinates inertia;
	Coordinates initial_position; // Для возвращения на старт, после столкновения с машиной
	Coordinates previous_position;
	void moveInDirection( Coordinates );
	bool isAlive;
	bool firstStep;
	bool secondStep;
	Coordinates convertFromDirectionCode( int );
};

