#include <string>
#include <iostream>
#include <thread>

#include "Core/Reader.h"
#include "Core/Game.h"
#include "GlobalDefinitions.h"
#include "UI/Drawing.h"

int main( int argc, char* argv[] )
{
	using namespace Core;

	CReader reader;
	try {
		CUIManager manager;
		size_t playerCount = manager.GetPlayersCount();
		size_t mapNumber = manager.GetMapNumber();
		CMap map = reader.ReadMap( RESOURCE_DIRECTORY + "Maps\\map" + std::to_string( mapNumber ) + ".txt" );
		CGame game( map, playerCount, manager );
		std::thread t( std::bind( UI::CDrawing::Draw, argc, argv ) );
		game.Start();

		glutDestroyWindow( UI::CDrawing::GetWindow() );
		t.join();
	} catch( std::exception const &e ) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}