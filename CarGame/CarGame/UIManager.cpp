#include <iostream>

#include "UIManager.h"
#include "UI/Drawing.h"

int CUIManager::GetDirection() const
{
	UI::CDrawing::DropKey();
	int key;
	while( (key = UI::CDrawing::GetKey()) == -1 ) {
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}
	return key;
}

void CUIManager::InitMap( const Core::CMap& map, const std::vector<Core::CPlayer>& players ) const
{
	Core::CField field = map.GetField();
	std::vector<std::vector<int>> newField;
	for( auto i = 0; i < field.size(); ++i ) {
		newField.push_back( std::vector<int>( 0 ) );
		for( auto j = 0; j < field[0].size(); ++j ) {
			newField[i].push_back( field[i][j] );
		}
	}

	std::vector<UI::CCar> cars;
	for( int i = 0; i < players.size(); ++i ) {
		auto playerCoordinates = players[i].GetPosition();
		cars.push_back( UI::CCar( UI::CCoordinates( playerCoordinates.x, playerCoordinates.y, PI / 2 ), UI::Color( i % 4 ) ) );
	}

	UI::CDrawing::Start();
	UI::CDrawing::Init( UI::CMap( newField ), cars );
}

void CUIManager::Move( const std::vector<Core::CPlayer>& movedPlayers ) const
{
	std::vector<int> numbers;
	std::vector<UI::CCoordinates> coordinates;
	for( auto player : movedPlayers ) {
		numbers.push_back( player.GetNumber() );
		coordinates.push_back( UI::CCoordinates( player.GetPosition().x, player.GetPosition().y ) );
	}
	UI::CDrawing::MoveCars( numbers, coordinates );
}

void CUIManager::ShowCollisions( const std::set<Core::CPlayer>& collidedPlayers ) const
{
	std::vector<int> numbers;
	std::vector<UI::CCoordinates> coordinates;
	for( auto player : collidedPlayers ) {
		numbers.push_back( player.GetNumber() );
		coordinates.push_back( UI::CCoordinates( player.GetInitialPosition().x, player.GetInitialPosition().y, PI / 2 ) );
	}
	UI::CDrawing::MoveCarsToStart( numbers, coordinates );
}

void CUIManager::ShowCrashes( const std::set<Core::CPlayer>& crashedPlayers ) const
{
	std::vector<int> numbers;
	for( auto player : crashedPlayers ) {
		numbers.push_back( player.GetNumber() );
	}
	UI::CDrawing::DeleteCars( numbers );
}

void CUIManager::ShowWinner( const Core::CPlayer* winner ) const
{
	if( winner == nullptr ) {
		std::cout << "All players are dead." << std::endl;
	} else {
		std::cout << "Player number " << winner->GetNumber() + 1 << " is winner! Congratulations!!!" << std::endl;
	}
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	UI::CDrawing::Stop();
}

size_t CUIManager::GetPlayersCount() const
{
	size_t result;
	std::cout << "Input players count..." << std::endl;
	std::cin >> result;
	return result;
}

size_t CUIManager::GetMapNumber() const
{
	size_t result;
	std::cout << "Input map number..." << std::endl;
	std::cin >> result;
	return result;
}