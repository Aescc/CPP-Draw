#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Brush.h"

class Canvas
{
public:
	Canvas( Graphics& gfx_in );
	~Canvas();
	void Update( Keyboard& kbd,Mouse& ms );
	void Draw() const;
	void Size( int x,int y,int w,int h );
private:
	void MovePixels( int xMove,int yMove );
	Color& GetPixel( int x,int y ) const;
private:
	void MakeCircle( int x,int y,int size,Color c );
	void ConnectLine( int x0,int y0,int x1,int y1,int in_size,Color c );
	void Swap( int& pos1,int& pos2 );
	float FindDist( int x0,int y0,int x1,int y1 ) const;
private:
	int x = 0;
	int y = 0;
	int oldX;
	int oldY;
	int width = gfx.ScreenWidth;
	int height = gfx.ScreenHeight;
	float size = 10.0f;
	Color curColor = Colors::Blue;
	static constexpr Color backgroundColor = Colors::White;
	Color *pixels;
	Brush brush;
	Graphics& gfx;
};