#pragma once

namespace UI {
	// координаты в игре
	struct CCoordinates {
		float x;
		float y;
		float angle;

		CCoordinates( float _x, float _y, float _angle = 0 ) :
			x( _x ),
			y( _y ),
			angle( _angle )
		{}
	};

	// координаты в окне
	struct CWindowCoordinates {
		float x;
		float y;
		CWindowCoordinates( float _x, float _y ) :
			x( _x ),
			y( _y )
		{}
	};
}