#include <thread>
#include <iostream>

#include <SOIL/SOIL.h>

#include "UI/Drawing.h"
#include "GlobalDefinitions.h"

namespace UI {
	CMap CDrawing::map; // static data members must be explicitly defined in exactly one translation unit
	std::vector<CCar> CDrawing::cars;
	bool CDrawing::initialized = false;
	bool CDrawing::started = false;
	std::mutex CDrawing::mutex;
	std::string CDrawing::windowName = "Rock'n'Roll Race";
	int CDrawing::window;
	int CDrawing::key;

	void CDrawing::Init( const CMap &mapData, const std::vector<CCar> &carsData )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( initialized ) {
			throw std::logic_error( "Game state has been already initialized" );
		}
		map = mapData;
		cars = carsData;
		initialized = true;
	}

	void CDrawing::Drop()
	{
		std::unique_lock<std::mutex> lock( mutex );
		initialized = false;
	}

	void CDrawing::reshape( int width, int height )
	{
		std::unique_lock<std::mutex> lock( mutex );
		glViewport( 0, 0, width, height ); // set view block

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0, width, 0, height ); // set coordinates 

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		map.Calculate(); // recalculate map
	}

	void CDrawing::timer( int value )
	{
		std::unique_lock<std::mutex> lock( mutex );
		if( !started ) {
			return;
		}
		glutPostRedisplay();
		glutTimerFunc( 1, timer, 0 );
	}

	std::string CDrawing::GetWindowName()
	{
		return windowName;
	}

	int CDrawing::GetWindow()
	{
		std::unique_lock<std::mutex> lock( mutex );
		return window;
	}

	void CDrawing::DropKey()
	{
		std::unique_lock<std::mutex> lock( mutex );
		key = -1;
	}

	int CDrawing::GetKey()
	{
		std::unique_lock<std::mutex> lock( mutex );
		return key;
	}

	void CDrawing::display()
	{
		std::unique_lock<std::mutex> lock( mutex );
		glClearColor( 1.0, 1.0, 1.0, 0.0 ); // clear background to white
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear buffers

		bool mapReloaded = !map.NeedToReload();
		map.Draw(); // Draw the map
		for( size_t i = 0; i < cars.size(); i++ ) {
			cars[i].Draw( map.GetCellSize(), map.GetIndent(), map.GetSize() ); // Draw car
		}
		glFlush(); // flush changes
		if( mapReloaded ) {
			glutSwapBuffers(); // if map wasn't reloaded (and buffers weren't swapped), swap buffers
		}
	}

	// load image from file to texture
	void CDrawing::loadTexture( const char* filename, GLuint& texture )
	{
		// create and choose texture
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		// read from file
		int width, height;
		int channels;
		unsigned char* image = SOIL_load_image( filename, &width, &height, &channels, SOIL_LOAD_RGBA );
		// save data to texture
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, float( GL_MODULATE ) );
		SOIL_free_image_data( image );
		// unchoose texture
		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	void CDrawing::MoveCars( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates )
	{
		std::unique_lock<std::mutex> lock( mutex );
		std::vector<CCoordinates> oldCoordinates;
		for( int i : numbers ) {
			oldCoordinates.push_back( cars[i].GetCoordinates() );
		}
		lock.unlock();
		const int fps = 100;
		for( int i = 0; i < numbers.size(); ++i ) {
			cars[numbers[i]].Rotate( oldCoordinates[i].x, oldCoordinates[i].y, newCoordinates[i].x, newCoordinates[i].y );
		}
		for( int j = 0; j <= fps; ++j ) {
			for( int i = 0; i < numbers.size(); ++i ) {
				cars[i].Move( UI::CCoordinates(
					float( oldCoordinates[numbers[i]].x * (fps - j) + newCoordinates[i].x * j ) / fps,
					float( oldCoordinates[numbers[i]].y * (fps - j) + newCoordinates[i].y * j ) / fps ) );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / fps ) );
		}
	}

	void CDrawing::MoveCarsToStart( const std::vector<int>& numbers, const std::vector<CCoordinates>& newCoordinates )
	{
		const int framesPerStep = 100;
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( 1.0f - float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
		for( int i = 0; i < numbers.size(); ++i ) {
			cars[numbers[i]].Move( newCoordinates[i] );
		}
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
	}

	void CDrawing::DeleteCars( const std::vector<int>& numbers )
	{
		const int framesPerStep = 100;
		for( int j = 0; j <= framesPerStep; ++j ) {
			for( int i : numbers ) {
				cars[i].SetOpacity( 1.0f - float( j ) / framesPerStep );
			}
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 / framesPerStep ) );
		}
		for( int i : numbers ) {
			cars[i].Crash();
		}
	}

	void CDrawing::Start()
	{
		std::unique_lock<std::mutex> lock( mutex );
		started = true;
	}

	void CDrawing::Stop()
	{
		std::unique_lock<std::mutex> lock( mutex );
		started = false;
	}

	bool CDrawing::Started()
	{
		std::unique_lock<std::mutex> lock( mutex );
		return started;
	}

	void CDrawing::load()
	{
		std::unique_lock<std::mutex> lock( mutex );
		//load textures for map
		loadTexture( (RESOURCE_DIRECTORY + "Images\\road.png").c_str(), map.textureRoad ); // road
		loadTexture( (RESOURCE_DIRECTORY + "Images\\forest.png").c_str(), map.textureBoard ); // board
																							//load textures for cars (depends on color)
		std::string carFilename;
		for( size_t i = 0; i < cars.size(); i++ ) {
			switch( cars[i].GetColor() ) {
				case RED:
					carFilename = RESOURCE_DIRECTORY + "Images\\car_red.png";
					break;
				case BLUE:
					carFilename = RESOURCE_DIRECTORY + "Images\\car_blue.png";
					break;
				case GREEN:
					carFilename = RESOURCE_DIRECTORY + "Images\\car_green.png";
					break;
				case ORANGE:
					carFilename = RESOURCE_DIRECTORY + "Images\\car_orange.png";
					break;
				default:
					carFilename = RESOURCE_DIRECTORY + "Images\\car_red.png";
			}
			loadTexture( carFilename.c_str(), cars[i].texture );
		}
	}

	void CDrawing::Draw( int argc, char** argv )
	{
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
		glutInitWindowSize( 800, 600 );
		while( !Started() ) {
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}
		std::unique_lock<std::mutex> lock( mutex );
		window = glutCreateWindow( "Rock'n'Roll Race" );
		lock.unlock();

		load();

		glutTimerFunc( 1, timer, 0 );
		glutReshapeFunc( reshape );
		glutDisplayFunc( display );
		glutKeyboardFunc( keyboardFunction );

		glutMainLoop();
	}

	void CDrawing::keyboardFunction( unsigned char pressedKey, int x, int y )
	{
		switch( pressedKey ) {
			case '1':
				key = 1;
				break;
			case '2':
				key = 2;
				break;
			case '3':
				key = 3;
				break;
			case '4':
				key = 4;
				break;
			case '5':
				key = 5;
				break;
			case '6':
				key = 6;
				break;
			case '7':
				key = 7;
				break;
			case '8':
				key = 8;
				break;
			case '9':
				key = 9;
				break;
			default:
				key = -1;
		}
	}
}