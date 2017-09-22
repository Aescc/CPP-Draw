#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include "Brush.h"

class Canvas
{
public:
	Canvas();
	~Canvas();
	void Update( Keyboard& kbd,Mouse& ms );
	void Draw( Graphics& gfx ) const;

	void SetNewSize( int x,int y,int w,int h );
private:
	void MovePixels( int xMove,int yMove );
	Color& GetPixel( int x,int y ) const;
private:
	void MakeCircle( int x,int y,int size,Color c );
	void ConnectLine( int x0,int y0,int x1,int y1,int in_size,Color c );

	void Swap( int& pos1,int& pos2 );
	int FindDist( int x0,int y0,int x1,int y1 ) const;
	int FindSlope( int x0,int y0,int x1,int y1 ) const;
private:
	int x = 0;
	int y = 0;

	int oldX;
	int oldY;

	int WIDTH  = Graphics::ScreenWidth;
	int HEIGHT = Graphics::ScreenHeight;

	float G_SIZE = 10.0f;
	Color U_COLOR = Colors::Blue;
	Color G_COLOR = Colors::Blue;
	static constexpr Color BG_COLOR = Colors::White;

	int PIXEL_NUM = WIDTH * HEIGHT;
	Color *pixels;
	Brush brush;
};