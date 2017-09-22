#include "Canvas.h"

Canvas::Canvas()
{
	pixels = new Color[PIXEL_NUM];

	for( int i = 0; i < PIXEL_NUM; ++i )
		pixels[i] = BG_COLOR;

	SetNewSize( 100,100,Graphics::ScreenWidth - 100,Graphics::ScreenHeight - 100 );
}

Canvas::~Canvas()
{
	delete[] pixels;
	pixels = nullptr;
}

void Canvas::Update( Keyboard& kbd,Mouse& ms )
{
	// Tool swap.
	if( kbd.KeyIsPressed( 66 ) )
		brush.Set( Brush::Tool::Brush );

	if( kbd.KeyIsPressed( 69 ) )
		brush.Set( Brush::Tool::Eraser );

	if( kbd.KeyIsPressed( 86 ) )
		brush.Set( Brush::Tool::Mover );

	if( kbd.KeyIsPressed( 73 ) )
		brush.Set( Brush::Tool::Identifier );

	const int SIZE_CHANGE = 2;
	// if( y > SIZE_CHANGE && HEIGHT < Graphics::ScreenHeight - SIZE_CHANGE )
	{
		if( kbd.KeyIsPressed( VK_UP ) )
			SetNewSize( x - SIZE_CHANGE,y,WIDTH,HEIGHT + SIZE_CHANGE );

		if( kbd.KeyIsPressed( VK_DOWN ) )
			SetNewSize( x + SIZE_CHANGE,y,WIDTH,HEIGHT - SIZE_CHANGE );
	}
	// if( x > SIZE_CHANGE && WIDTH < Graphics::ScreenWidth - SIZE_CHANGE )
	{
		if( kbd.KeyIsPressed( VK_LEFT ) )
			SetNewSize( x,y + SIZE_CHANGE,WIDTH - SIZE_CHANGE,HEIGHT );

		if( kbd.KeyIsPressed( VK_RIGHT ) )
			SetNewSize( x,y - SIZE_CHANGE,WIDTH + SIZE_CHANGE,HEIGHT );
	}

	float brushSizeChange = 1.0f;

	if( G_SIZE < 10 )
		brushSizeChange = 0.3f;

	if( kbd.KeyIsPressed( 219 ) && G_SIZE > 1 + brushSizeChange )
		G_SIZE -= brushSizeChange;

	if( kbd.KeyIsPressed( 221 ) )
		G_SIZE += brushSizeChange;

	if( ms.LeftIsPressed() )
	{
		if( brush.CurTool() != Brush::Tool::Mover &&
			brush.CurTool() != Brush::Tool::Identifier )
		{
			if( brush.CurTool() == Brush::Tool::Brush )
				G_COLOR = U_COLOR;

			if( brush.CurTool() == Brush::Tool::Eraser )
				G_COLOR = BG_COLOR;

			MakeCircle( ms.GetPosX(),ms.GetPosY(),int( G_SIZE ),G_COLOR );
			ConnectLine( oldX,oldY,ms.GetPosX(),ms.GetPosY(),int( G_SIZE ),G_COLOR );
		}
		else if( brush.CurTool() == Brush::Tool::Mover )
			MovePixels( oldX - ms.GetPosX(),oldY - ms.GetPosY() );
		else if( brush.CurTool() == Brush::Tool::Identifier )
			U_COLOR = pixels[ms.GetPosY() * WIDTH + ms.GetPosX()];
	}

	oldX = ms.GetPosX();
	oldY = ms.GetPosY();
}

void Canvas::Draw( Graphics& gfx ) const
{
	for( int i = x; i < HEIGHT; ++i )
	{
		for( int j = y; j < WIDTH; ++j )
			gfx.PutPixel( j,i,pixels[i * WIDTH + j] );
	}

	Color mouseColor = G_COLOR;

	if( brush.CurTool() == Brush::Tool::Eraser )
		mouseColor = Colors::Red;

	if( brush.CurTool() == Brush::Tool::Identifier )
		mouseColor = Colors::Green;

	if( brush.CurTool() == Brush::Tool::Mover )
		mouseColor = Colors::Gray;

	if( oldX - G_SIZE > 0 && oldX + G_SIZE < Graphics::ScreenWidth &&
		oldY - G_SIZE > 0 && oldY + G_SIZE < Graphics::ScreenHeight )
		gfx.DrawCircle( oldX,oldY,G_SIZE,mouseColor );
}

void Canvas::SetNewSize( int in_x,int in_y,int in_w,int in_h )
{
	if( in_x > 0 && in_x < in_h && in_y > 0 && in_y < in_w )
	{
		x = in_x;
		y = in_y;
		WIDTH = in_w;
		HEIGHT = in_h;

		PIXEL_NUM = WIDTH * HEIGHT;

		for( int i = 0; i < PIXEL_NUM; ++i )
			pixels[i] = BG_COLOR;
	}
}

void Canvas::MovePixels( int xMove,int yMove )
{
	for( int i = 1; i < HEIGHT + 1; ++i )
	{
		for( int j = 1; j < WIDTH + 1; ++j )
		{
			GetPixel( j - 1,i - 1 ) = Colors::Magenta;
		}
	}
}

Color& Canvas::GetPixel( int x,int y ) const
{
	return pixels[y * WIDTH + x];
}

void Canvas::MakeCircle( int x,int y,int size,Color c )
{
	const int rad_sq = size * size;
	for( int i = y - size; i < y + size; ++i )
	{
		for( int j = x - size; j < x + size; ++j )
		{
			const int xDiff = x - j;
			const int yDiff = y - i;

			if( xDiff * xDiff + yDiff * yDiff < rad_sq &&
				i * WIDTH + j > 0 && i * WIDTH + j < PIXEL_NUM )
				pixels[i * WIDTH + j] = c;
		}
	}
}

void Canvas::ConnectLine( int x0,int y0,int x1,int y1,int in_size,Color c )
{
	if( y0 == y1 )
	{
		if( x0 > x1 )
		{
			int tempX = x0;
			x0 = x1;
			x1 = tempX;
			int tempY = y0;
			y0 = y1;
			y1 = tempY;
		}
		for( int i = x0; i < x1; ++i )
			MakeCircle( i,y0,in_size,c );
		return;
	}

	if( x0 == x1 )
	{
		if( y0 > y1 )
		{
			int tempX = x0;
			x0 = x1;
			x1 = tempX;
			int tempY = y0;
			y0 = y1;
			y1 = tempY;
		}
		for( int i = y0; i < y1; ++i )
			MakeCircle( x0,i,in_size,c );
		return;
	}

	int distance = FindDist( x0,y0,x1,y1 ) + 1;

	float ySlope = (float)( y1 - y0 ) / (float)distance;
	float xSlope = (float)( x1 - x0 ) / (float)distance;

	for( int i = 0; i < distance; ++i )
	{
		float newX = x0 + i * xSlope + 0.5f;
		float newY = y0 + i * ySlope + 0.5f;
		MakeCircle( int( newX ),int( newY ),in_size,c );
	}
}

void Canvas::Swap( int& pos1,int& pos2 )
{
	const int temp = pos1;
	pos1 = pos2;
	pos2 = temp;
}

int Canvas::FindDist( int x0,int y0,int x1,int y1 ) const
{
	const float deltaX = float( x1 ) - float( x0 );
	const float deltaY = float( y1 ) - float( y0 );

	const float dist = sqrt( ( deltaX * deltaX ) + ( deltaY * deltaY ) );
	return int( dist );
}

int Canvas::FindSlope( int x0,int y0,int x1,int y1 ) const
{
	const int deltaX = x1 - x0;
	const int deltaY = y1 - y0;

	if( deltaX == 0 )
		return 0;

	const int slope = deltaY / deltaX;
	return    slope;
}