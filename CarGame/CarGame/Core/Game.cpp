#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>

#include "Core/Game.h"

namespace Core {
	namespace {
		bool inBoxOnAxis( int firstPoint, int secondPoint, int thirdPoint, int forthPoint )
		{
			if( firstPoint > secondPoint ) {
				std::swap( firstPoint, secondPoint );
			}
			if( thirdPoint > forthPoint ) {
				std::swap( thirdPoint, forthPoint );
			}
			return std::max( firstPoint, thirdPoint ) <= std::min( secondPoint, forthPoint );
		}

		int area( const CCoordinates& firstPoint, const CCoordinates& secondPoint, const CCoordinates& thirdPoint )
		{
			return (secondPoint.x - firstPoint.x) * (thirdPoint.y - firstPoint.y)
				- (secondPoint.y - firstPoint.y) * (thirdPoint.x - firstPoint.x);
		}

		bool isIntersects(
			const CCoordinates& firstPoint,
			const CCoordinates& secondPoint,
			const CCoordinates& thirdPoint,
			const CCoordinates& fourthPoint )
		{
			return inBoxOnAxis( firstPoint.x, secondPoint.x, thirdPoint.x, fourthPoint.x )
				&& inBoxOnAxis( firstPoint.y, secondPoint.y, thirdPoint.y, fourthPoint.y )
				&& (area( firstPoint, secondPoint, thirdPoint )
					* area( firstPoint, secondPoint, fourthPoint )) <= 0
				&& (area( thirdPoint, fourthPoint, firstPoint )
					* area( thirdPoint, fourthPoint, secondPoint )) <= 0;
		}
	}

	CGame::CGame( const CMap& newMap, size_t playersNumber, const CUIManager& _manager ) :
		map( newMap ),
		numOfDeadPlayers( 0 ),
		manager( _manager )
	{
		if( playersNumber > map.GetStartPoints().size() ) {
			throw std::invalid_argument( std::string( "Too many players. This map is for " ) + std::to_string( map.GetStartPoints().size() ) + " players or less." );
		}
		for( int i = 0; i < playersNumber; ++i ) {
			players.push_back( CPlayer( map.GetStartPoints()[i], i ) );
		}
	}

	int CGame::finishLineIntersectsWithPlayer( const CPlayer& player ) const
	{
		// Происходит проверка:
		// 1. Проекции отрезков на оси пересекаются
		// 2. Считается ориентированная площадь треугольников. Нужно, чтобы эти площади были разных знаков.
		CCoordinates playersPreviousCoordinates = player.GetPreviousPosition();
		CCoordinates playersCoordinates = player.GetPosition();

		if (playersCoordinates == playersPreviousCoordinates)
			return 0;

		if ( isIntersects( playersPreviousCoordinates, playersCoordinates, map.GetFinishLine().first, map.GetFinishLine().second ) )
		{
			CCoordinates normalVectorCoordinates( map.GetFinishLine().second.x - map.GetFinishLine().first.x,
				map.GetFinishLine().second.y - map.GetFinishLine().first.y );
			if (area(map.GetFinishLine().first, playersCoordinates, map.GetFinishLine().second)
				* area(map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second) > 0)
				return 1;
			else if (area(map.GetFinishLine().first, playersCoordinates, map.GetFinishLine().second) == 0
				&& area(map.GetFinishLine().first, playersPreviousCoordinates, map.GetFinishLine().second)
				* area(map.GetFinishLine().first, normalVectorCoordinates, map.GetFinishLine().second) < 0)
				return 1;
				else
					return -1;
		}
		else return 0;
	}

	const CPlayer* CGame::handleFinishLineIntersections()
	{
		for( int i = 0; i < players.size(); ++i ) {
			if( finishLineIntersectsWithPlayer( players[i] ) == -1 ) {
				players[i].StartCheating();
			}
			if( finishLineIntersectsWithPlayer( players[i] ) == 1 ) {
				if (players[i].IsCheating() ) {
					players[i].StopCheating();
				} else {
					return &players[i];
				}
			}
		}
		return nullptr;
	}

	bool CGame::playerOutOfTrack( const CPlayer& player ) const
	{
		CCoordinates playersPreviousCoordinates = player.GetPreviousPosition();
		CCoordinates playersCoordinates = player.GetPosition();

		int minX = std::min( playersPreviousCoordinates.x, playersCoordinates.x ),
			maxX = std::max( playersPreviousCoordinates.x, playersCoordinates.x ),
			minY = std::min( playersPreviousCoordinates.y, playersCoordinates.y ),
			maxY = std::max( playersPreviousCoordinates.y, playersCoordinates.y );

		CCoordinates realCoordinates( playersCoordinates.x * 10 + 5, playersCoordinates.y * 10 + 5 ),
			realPreviousCoordinates( playersPreviousCoordinates.x * 10 + 5, playersPreviousCoordinates.y * 10 + 5 );

		for( int i = minX; i <= maxX; ++i ) {
			for( int j = minY; j <= maxY; ++j ) {
				if( map.IsEmpty( i, j ) ) {
					continue;
				}
				CCoordinates firstPoint( i * 10, j * 10 ),
					secondPoint( (i + 1) * 10, j * 10 ),
					thirdPoint( (i + 1) * 10, (j + 1) * 10 ),
					fourthPoint( i * 10, (j + 1) * 10 );

				if( isIntersects( realPreviousCoordinates, realCoordinates, firstPoint, secondPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, secondPoint, thirdPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, thirdPoint, fourthPoint ) ||
					isIntersects( realPreviousCoordinates, realCoordinates, fourthPoint, firstPoint ) ) {
					return true;
				}
			}
		}
		return false;
	}

	void CGame::findCollisions()
	{
		for( size_t i = 0; i < players.size(); ++i ) {
			for( size_t j = i + 1; j < players.size(); ++j ) {
				if ( players[i].GetPosition() == players[j].GetPosition() && players[i].IsAlive() && players[j].IsAlive() ) {
					collidedPlayers.insert( players[i] );
					collidedPlayers.insert( players[j] );
				}
			}
		}
	}

	void CGame::findCrashes()
	{
		for( auto player : players ) {
			if( !player.IsAlive() ) {
				continue;
			}
			if( playerOutOfTrack( player ) && player.IsAlive() ) {
				crashedPlayers.insert( player );
			}
		}
	}

	void CGame::turnOfPlayer( CPlayer& player )
	{
		int direction = manager.GetDirection();
		if( player.DirectionIsValid( Direction( direction ), map.GetSize() ) ) {
			player.Move( Direction( direction ) );
			return;
		}
		player.Move( Direction( direction ) );
		player.Die();
		crashedPlayers.insert( player );
	}

	void CGame::Start()
	{
		std::cout << "Game has been started." << std::endl;
		const CPlayer* winner = nullptr;
		manager.InitMap( map, players );

		do {
			for( size_t i = 0; i < players.size(); ++i ) {
				// в далёком будущем здесь будет посылаться сообщение UI, чтобы он отрисовал возможные ходы, выделил ходящего игрока и т.д., но лекторы об этом знать не должны
				if( players[i].IsAlive() ) {
					turnOfPlayer( players[i] ); // AI: Если будет AI, здесь он запускается (перед этим, занести его в players[])
				}
			}

			manager.Move( players );

			findCollisions();
			findCrashes();

			for( auto crashedPlayer : crashedPlayers ) {
				auto it = collidedPlayers.find( crashedPlayer );
				if( it != collidedPlayers.end() ) {
					collidedPlayers.erase( it );
				}
			}
			for( auto collidedPlayer : collidedPlayers ) {
				players[collidedPlayer.GetNumber()].GoToStart();
			}
			for( auto crashedPlayer : crashedPlayers ) {
				players[crashedPlayer.GetNumber()].Die();
			}
			if( !collidedPlayers.empty() ) {
				manager.ShowCollisions( collidedPlayers );
			}
			if( !crashedPlayers.empty() ) {
				manager.ShowCrashes( crashedPlayers );
			}

			numOfDeadPlayers += crashedPlayers.size();
			if( numOfDeadPlayers == players.size() ) {
				break;
			}

			collidedPlayers.clear();
			crashedPlayers.clear();
		} while( (winner = handleFinishLineIntersections()) == nullptr );

		finish( winner );
	}

	void CGame::finish( const CPlayer* winnerPtr )
	{
		manager.ShowWinner( winnerPtr );
	}
}