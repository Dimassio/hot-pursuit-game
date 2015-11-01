#include <string>
#include "CGame.h"

const std::string MAP_PATH = "../map.txt";

int main()
{
	Reader reader;
	try {
		Map newMap( reader.readMap( MAP_PATH ) );
		Line startLine = reader.readLine();
		PlayersInfo newPlayersInfo = reader.readPlayers();
		Game newGame( newMap, newPlayersInfo, startLine, reader );
		newGame.start();
	} catch( std::exception const &e ) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}