#include "Core/Player.h"

namespace Core {
	CPlayer::CPlayer( const CCoordinates& coordinates, size_t playerNumber ) :
		position( coordinates ),
		inertia( 0, 0 ),
		initialPosition( coordinates ),
		previousPosition( coordinates ),
		isAlive( true ),
		isCheating( false ),
		number( playerNumber )
	{}

	void CPlayer::GoToStart()
	{
		this->position = this->initialPosition;
		this->previousPosition = this->initialPosition;
		this->inertia.x = 0;
		this->inertia.y = 0;
	}

	void CPlayer::StartCheating()
	{
		isCheating = true;
	}

	void CPlayer::StopCheating()
	{
		isCheating = false;
	}

	CCoordinates CPlayer::convertFromDirectionCode( Direction directionCode ) const
	{
		switch( directionCode ) {
			case SW:
				return CCoordinates( -1, 1 );
			case S:
				return CCoordinates( 0, 1 );
			case SE:
				return CCoordinates( 1, 1 );
			case W:
				return CCoordinates( -1, 0 );
			case C:
				return CCoordinates( 0, 0 );
			case E:
				return CCoordinates( 1, 0 );
			case NW:
				return CCoordinates( -1, -1 );
			case N:
				return CCoordinates( 0, -1 );
			case NE:
				return CCoordinates( 1, -1 );
			default:
				throw std::invalid_argument( "Unknown direction code" );
		}
	}

	void CPlayer::Move( Direction direction_code )
	{
		CCoordinates direction = convertFromDirectionCode( direction_code );
		CCoordinates move( inertia.x + direction.x, inertia.y + direction.y );

		inertia = move;
		previousPosition = position;
		position.x += move.x;
		position.y += move.y;
	};

	CCoordinates CPlayer::GetPreviousPosition() const
	{
		return previousPosition;
	}

	CCoordinates CPlayer::GetInitialPosition() const
	{
		return initialPosition;
	}

	CCoordinates CPlayer::GetPosition() const
	{
		return position;
	}

	size_t CPlayer::GetNumber() const
	{
		return number;
	}

	void CPlayer::Die()
	{
		isAlive = false;
	};

	bool CPlayer::DirectionIsValid( Direction directionCode, const CSize& size ) const
	{
		CCoordinates direction = convertFromDirectionCode( directionCode );
		return 0 <= (direction + position + inertia).x && (direction + position + inertia).x <= size.first - 1 &&
			0 <= (direction + position + inertia).y && (direction + position + inertia).y <= size.second - 1;
	}

	bool CPlayer::IsAlive() const
	{
		return isAlive;
	}

	bool CPlayer::IsCheating() const
	{
		return isCheating;
	}

	bool CPlayer::operator< ( const CPlayer& player ) const
	{
		return number < player.number;
	}
}