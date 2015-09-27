#include <string>
#include "CGame.h"

const std::string MAP_PATH = ""; // ���� � �����

int main()
{
	Reader reader;
	Map newMap( reader.readMap( MAP_PATH ) );
	PlayersInfo newPlayersInfo = reader.readPlayers();
	Game newGame( newMap, newPlayersInfo, reader );
	newGame.start();

	return 0;
}