#pragma once

#include "GlobalDefinitions.h"

namespace Core {
	enum Direction {
		SW = 1, S, SE,
		W, C, E, // `C` for no direction
		NW, N, NE
	};

	class CPlayer {
	public:
		CPlayer( const CCoordinates& coordinates, size_t playerNumber );

		void Die();
		void GoToStart();
		void StartCheating();
		void StopCheating();

		void Move( Direction direction );
		bool DirectionIsValid( Direction direction, const CSize& size ) const;
		
		CCoordinates GetPosition() const;
		CCoordinates GetPreviousPosition() const;
		CCoordinates GetInitialPosition() const;
		size_t GetNumber() const;
		bool IsAlive() const;
		bool IsCheating() const;

		bool operator< ( const CPlayer& player ) const;
	private:
		CCoordinates position;
		CCoordinates inertia;
		CCoordinates initialPosition; // Для возвращения на старт, после столкновения с машиной
		CCoordinates previousPosition;
		bool isAlive;
		bool isCheating;
		size_t number;

		CCoordinates convertFromDirectionCode( Direction directionCode ) const;
	};
}