#include <memory>

#include "UI/Map.h"

namespace UI {
	CMap::CMap() :
		textureBoard( 0 ),
		textureRoad( 0 ),
		textureMap( 0 ),
		cellSize( 0 ),
		indent( 0, 0 ),
		needReload( 0 )
	{}

	CMap::CMap( const std::vector<std::vector<int>>& map_data ) :
		textureBoard( 0 ),
		textureRoad( 0 ),
		textureMap( 0 ),
		map( map_data ),
		cellSize( 0 ),
		indent( 0, 0 ),
		needReload( 0 )
	{
		glGenTextures( 1, &textureMap );
		glGenTextures( 1, &textureBoard );
		glGenTextures( 1, &textureRoad );
		needReload = true;
	}

	void CMap::Calculate()
	{
		int n = map.size(), m = map[0].size();
		float height = glutGet( GLUT_WINDOW_HEIGHT ),
			width = glutGet( GLUT_WINDOW_WIDTH );
		cellSize = fmin( height / n, width / m ); // the length of one little square - "cell"
		indent.x = (width - cellSize * m) / 2; // indent from left and right window sides
		indent.y = (height - cellSize * n) / 2;  // indent from top and bottom window sides
		needReload = true; // need to reload map
	}

	void CMap::saveTexture()
	{
		// write window to array of pixels
		unsigned long imageSize;
		GLint viewPort[4];
		glGetIntegerv( GL_VIEWPORT, viewPort );
		GLint width = viewPort[2];
		GLint height = viewPort[3];
		imageSize = ((width + ((4 - (width % 4)) % 4)) * height * 3);
		std::shared_ptr<GLbyte> data = std::shared_ptr<GLbyte>( new GLbyte[imageSize] );
		glReadBuffer( GL_FRONT );
		glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );

		// write pixels to texture of map
		glBindTexture( GL_TEXTURE_2D, textureMap );
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get() );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, float( GL_MODULATE) );
	}

	void CMap::reload()
	{
		int n = map.size(), m = map[0].size();
		for( int i = 0; i < n; i++ ) {
			for( int j = 0; j < m; j++ ) {
				glEnable( GL_TEXTURE_2D );
				if( map[i][j] == 1 ) {
					glBindTexture( GL_TEXTURE_2D, textureBoard ); // load a texture of board (forest)
				} else {
					glBindTexture( GL_TEXTURE_2D, textureRoad ); // load a texture of road
				}
				//calculate coordinates
				float left = j * cellSize + indent.x;
				float right = (j + 1) * cellSize + indent.x;
				float bottom = (n - i) * cellSize + indent.y;
				float top = (n - (i + 1)) * cellSize + indent.y;
				//Draw a cell with texture (board or road)
				glBegin( GL_POLYGON );
				glTexCoord2f( 0.0f, 0.0f ); glVertex3f( left, bottom, 0.0f );
				glTexCoord2f( 1.0f, 0.0f ); glVertex3f( right, bottom, 0.0f );
				glTexCoord2f( 1.0f, 1.0f ); glVertex3f( right, top, 0.0f );
				glTexCoord2f( 0.0f, 1.0f ); glVertex3f( left, top, 0.0f );
				glEnd();
			}
		}
		glutSwapBuffers();
		saveTexture(); // save the whole window with map to texture
		needReload = false;
	}

	void CMap::Draw()
	{
		if( needReload ) {
			reload();
			return;
		}
		// load window from texture
		glEnable( GL_TEXTURE_2D );
		int height = glutGet( GLUT_WINDOW_HEIGHT ),
			width = glutGet( GLUT_WINDOW_WIDTH );
		// choose texture
		glBindTexture( GL_TEXTURE_2D, textureMap );
		// Draw a polygon of window size with texture
		glBegin( GL_POLYGON );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 0, 0, 0.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( width, 0, 0.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( width, height, 0.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 0, height, 0.0f );
		glEnd();
	}

	float CMap::GetCellSize() const
	{
		return cellSize;
	}

	CWindowCoordinates CMap::GetIndent() const
	{
		return indent;
	}

	CSize CMap::GetSize() const
	{
		return CSize( map[0].size(), map.size() );
	}

	bool CMap::NeedToReload()
	{
		return needReload;
	}
}