#pragma once
#include <vector>
#include <GL/glut.h>

#include "UI/Coord.h"
#include "GlobalDefinitions.h"

namespace UI {
	class CMap {
	public:
		CMap();
		CMap( const std::vector<std::vector<int>> &mapData );
		void Calculate();
		void Draw();
		float GetCellSize() const;
		CWindowCoordinates GetIndent() const;
		CSize GetSize() const;

		bool NeedToReload();

		GLuint textureBoard;
		GLuint textureRoad;
		GLuint textureMap;

	private:
		std::vector<std::vector<int>> map;
		float cellSize;
		CWindowCoordinates indent;
		bool needReload;

		void saveTexture();
		void reload();
	};
}