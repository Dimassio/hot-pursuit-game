#pragma once

#include <set>

#include "Core/Map.h"
#include "Core/Player.h"
#include "UIManager.h"

namespace Core {
	class CGame {
	public:
		CGame( const CMap& map, size_t playersNumber, const CUIManager& _manager );

		void Start();

	private:
		CMap map;
		size_t numOfDeadPlayers;
		std::vector<CPlayer> players;
		// все игроки, которые врезались друг в друга
		std::set<CPlayer> collidedPlayers;
		// все игроки, которые во что-то въехали
		std::set<CPlayer> crashedPlayers;
		CUIManager manager;

		const CPlayer* handleFinishLineIntersections();
		// -1 - если пересекли финишную черту в неправильную сторону, 0 - не пересекли, 1 - в правильную сторону
		int finishLineIntersectsWithPlayer( const CPlayer& player ) const;
		void turnOfPlayer( CPlayer& player );
		bool playerOutOfTrack( const CPlayer& player ) const;
		void findCollisions();
		void findCrashes();
		void finish( const CPlayer* player = nullptr );
	};
}