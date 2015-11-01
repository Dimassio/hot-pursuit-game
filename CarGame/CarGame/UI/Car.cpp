#include <vector>

#include "UI/Car.h"
#include <iostream>

namespace UI {
	namespace {
		float find_angle( int x, int y )
		{
			float angle = 0.0;
			int length = x * x + y * y;
			if( length != 0 ) {
				angle = float( x ) / sqrt( length );
			}
			return acos( angle );
		}
	}

	CCar::CCar( CCoordinates startCoordinates, Color carColor ) :
		coords( startCoordinates ),
		color( carColor ),
		crashed( false ),
		alpha( 1.0f )
	{}

	Color CCar::GetColor() const
	{
		return color;
	}

	CCoordinates CCar::GetCoordinates() const
	{
		return coords;
	}

	void CCar::rotate( float &x, float &y, float angle ) const
	{
		float tx = x;
		float ty = y;
		x = tx * cos( angle ) - ty * sin( angle );
		y = tx * sin( angle ) + ty * cos( angle );
	}

	void CCar::rotateCar( float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &Dx, float &Dy, float centerX, float centerY, float angle ) const
	{
		Ax -= centerX;
		Bx -= centerX;
		Cx -= centerX;
		Dx -= centerX;

		Ay -= centerY;
		By -= centerY;
		Cy -= centerY;
		Dy -= centerY;

		rotate( Ax, Ay, angle );
		rotate( Bx, By, angle );
		rotate( Cx, Cy, angle );
		rotate( Dx, Dy, angle );

		Ax += centerX;
		Bx += centerX;
		Cx += centerX;
		Dx += centerX;

		Ay += centerY;
		By += centerY;
		Cy += centerY;
		Dy += centerY;
	}

	void CCar::Draw( float cellSize, CWindowCoordinates indent, CSize mapSize )
	{
		if( crashed ) {
			return;
		}
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexEnvf( GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		CWindowCoordinates cord = transateToWcoord( coords.x, coords.y, cellSize, indent, mapSize );
		float left = cord.x;
		float right = cord.x + cellSize;
		float bottom = cord.y - cellSize / 4;
		float top = cord.y - 3 * cellSize / 4;

		glDepthMask( GL_FALSE );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glBegin( GL_POLYGON );
		{
			glColor4f( 1, 1, 1, alpha );
			float Ax = left,
				Ay = bottom,
				Bx = right,
				By = bottom,
				Cx = right,
				Cy = top,
				Dx = left,
				Dy = top;

			float centerX = Dx - (Dx - Bx) / 2,
				centerY = Dy - (Dy - By) / 2;
			rotateCar( Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, centerX, centerY, coords.angle );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( Ax, Ay, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( Bx, By, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( Cx, Cy, 0.0f );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( Dx, Dy, 0.0f );
		}
		glEnd();
		glDisable( GL_BLEND );
		glDepthMask( GL_TRUE );
	}

	void CCar::Move( CCoordinates coordinates )
	{
		coords.x = coordinates.x;
		coords.y = coordinates.y;
	}

	void CCar::Crash()
	{
		crashed = true;
	}

	void CCar::SetOpacity( float opacity )
	{
		alpha = opacity;
	}

	void CCar::Rotate( float x1, float y1, float x2, float y2 )
	{
		if( x1 == x2 && y1 == y2 ) {
			return;
		}
		double cos = (x2 - x1) / std::sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
		double sin = -(y2 - y1) / std::sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) ); // минус из-за обратных координат в glut
		if( cos < 0 && sin < 0 ) {
			coords.angle = PI * 2 - std::acos( cos );
		} else if( cos < 0 && sin >= 0 ) {
			coords.angle = std::acos( cos );
		} else if( cos >= 0 && sin < 0 ) {
			coords.angle = PI * 2 - std::acos( cos );
		} else {
			coords.angle = std::acos( cos );
		}
	}

	CWindowCoordinates CCar::transateToWcoord( float x, float y, float cellSize, CWindowCoordinates indent, CSize mapSize ) const
	{
		CWindowCoordinates wcoord( 0, 0 );
		wcoord.x = x * cellSize + indent.x;
		wcoord.y = (mapSize.second - y) * cellSize + indent.y;
		return wcoord;
	}
}