#include "Canvas.h"
#include <assert.h>

Canvas::Canvas( Graphics& gfx_in )
	:
	gfx( gfx_in )
{
	pixels = new Color[width * height];
	for( int i = 0; i < width * height; ++i )
	{
		pixels[i] = backgroundColor;
	}
	Size( 100,100,Graphics::ScreenWidth - 100,Graphics::ScreenHeight - 100 );
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
	{
		brush.Set( Brush::Tool::Brush );
	}
	if( kbd.KeyIsPressed( 69 ) )
	{
		brush.Set( Brush::Tool::Eraser );
	}
	if( kbd.KeyIsPressed( 73 ) )
	{
		brush.Set( Brush::Tool::Identifier );
	}
	float brushSizeChange = 1.0f;
	if( size < 10.0f )
	{
		brushSizeChange = 0.3f;
	}
	if( kbd.KeyIsPressed( 219 ) && size > 1.0f + brushSizeChange )
	{
		size -= brushSizeChange;
	}
	if( kbd.KeyIsPressed( 221 ) )
	{
		size += brushSizeChange;
	}

	MouseOnCorner( ms );
	if( ms.LeftIsPressed() )
	{
		if( brush.CurTool() == Brush::Tool::Brush )
		{
			// curColor = U_COLOR;
			MakeCircle( ms.GetPosX(),ms.GetPosY(),int( size ),curColor );
			ConnectLine( oldX,oldY,ms.GetPosX(),ms.GetPosY(),int( size ),curColor );
		}
		else if( brush.CurTool() == Brush::Tool::Eraser )
		{
			MakeCircle( ms.GetPosX(),ms.GetPosY(),int( size ),backgroundColor );
			ConnectLine( oldX,oldY,ms.GetPosX(),ms.GetPosY(),int( size ),backgroundColor );
		}
		else if( brush.CurTool() == Brush::Tool::Identifier )
		{
			if( ms.GetPosX() >= x && ms.GetPosX() < width &&
				ms.GetPosY() >= y && ms.GetPosY() < height )
			{
				curColor = GetPixel( ms.GetPosX(),ms.GetPosY() );
			}
		}
		else if( brush.CurTool() == Brush::Tool::Resizer )
		{
			if( mouseResizePos == MouseResizer::TopLeft )
			{
				Size( ms.GetPosX(),ms.GetPosY(),width,height );
			}
			else if( mouseResizePos == MouseResizer::TopRight )
			{
				Size( x,ms.GetPosY(),ms.GetPosX(),height );
			}
			else if( mouseResizePos == MouseResizer::BotLeft )
			{
				Size( ms.GetPosX(),y,width,ms.GetPosY() );
			}
			else if( mouseResizePos == MouseResizer::BotRight )
			{
				Size( x,y,ms.GetPosX(),ms.GetPosY() );
			}
		}
	}

	oldX = ms.GetPosX();
	oldY = ms.GetPosY();
}

void Canvas::Draw() const
{
	for( int i = y; i < height; ++i )
	{
		for( int j = x; j < width; ++j )
		{
			gfx.PutPixel( j,i,pixels[i * width + j] );
		}
	}
	Color mouseColor = curColor;
	if( brush.CurTool() == Brush::Tool::Eraser )
	{
		mouseColor = Colors::Red;
	}
	if( brush.CurTool() == Brush::Tool::Identifier )
	{
		mouseColor = Colors::Green;
	}
	if( brush.CurTool() == Brush::Tool::Resizer )
	{
		mouseColor = Colors::Gray;
	}

	// if( oldX - int( size ) >= 0 && oldX + int( size ) < Graphics::ScreenWidth &&
	// 	oldY - int( size ) >= 0 && oldY + int( size ) < Graphics::ScreenHeight )
	{
		gfx.DrawCircleSafe( oldX,oldY,int( size ),mouseColor );
	}
}

void Canvas::Size( int in_x,int in_y,int in_w,int in_h )
{
	if( in_x > 0 && in_x < in_h && in_y > 0 && in_y < in_w )
	{
		x = in_x;
		y = in_y;
		width = in_w;
		height = in_h;
		// PIXEL_NUM = WIDTH * HEIGHT;
		for( int i = 0; i < width * height; ++i )
		{
			pixels[i] = backgroundColor;
		}
	}
}

Color& Canvas::GetPixel( int x_in,int y_in ) const
{
	assert( x_in >= x );
	assert( x_in < width );
	assert( y_in >= y );
	assert( y_in < height );
	return pixels[y * width + x];
}

void Canvas::MakeCircle( int x,int y,int size,Color c ) const
{
	const int radSq = size * size;
	for( int i = y - size; i < y + size; ++i )
	{
		for( int j = x - size; j < x + size; ++j )
		{
			if( j >= this->x && j < this->width &&
				i >= this->y && i < this->height )
			{
				const int xDiff = x - j;
				const int yDiff = y - i;
				if( xDiff * xDiff + yDiff * yDiff < radSq &&
					i * width + j > 0 && i * width + j < width * height &&
					GetPixel( j,i ) != c )
				{
					pixels[i * width + j] = c;
				}
			}
		}
	}
}

void Canvas::ConnectLine( int x0,int y0,int x1,int y1,int in_size,Color c ) const
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
		{
			MakeCircle( i,y0,in_size,c );
		}
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
		{
			MakeCircle( x0,i,in_size,c );
		}
		return;
	}
	float distance = FindDist( x0,y0,x1,y1 ) + 1;
	float ySlope = ( float )( y1 - y0 ) / distance;
	float xSlope = ( float )( x1 - x0 ) / distance;
	for( int i = 0; i < int( distance ); ++i )
	{
		const float newX = x0 + 0.5f + i * xSlope;
		const float newY = y0 + 0.5f + i * ySlope;
		MakeCircle( int( newX ),int( newY ),in_size,c );
	}
}

float Canvas::FindDist( int x0,int y0,int x1,int y1 ) const
{
	const float deltaX = float( x1 - x0 );
	const float deltaY = float( y1 - y0 );
	const float dist = sqrt( ( deltaX * deltaX ) + ( deltaY * deltaY ) );
	return dist;
}

bool Canvas::MouseOnCorner( const Mouse& ms )
{
	// TODO: Make these if/else ifs less gross and less verbose.
	if( brush.CurTool() != Brush::Tool::Resizer )
	{
		lastTool = brush.CurTool();
	}
	const float maxClickableDistance = 20.0f;
	if( FindDist( x,y,ms.GetPosX(),ms.GetPosY() ) <= maxClickableDistance )
	{
		brush.Set( Brush::Tool::Resizer );
		mouseResizePos = MouseResizer::TopLeft;
	}
	else if( FindDist( width,y,ms.GetPosX(),ms.GetPosY() ) <= maxClickableDistance )
	{
		brush.Set( Brush::Tool::Resizer );
		mouseResizePos = MouseResizer::TopRight;
	}
	else if( FindDist( x,height,ms.GetPosX(),ms.GetPosY() ) <= maxClickableDistance )
	{
		brush.Set( Brush::Tool::Resizer );
		mouseResizePos = MouseResizer::BotLeft;
	}
	else if( FindDist( width,height,ms.GetPosX(),ms.GetPosY() ) <= maxClickableDistance )
	{
		brush.Set( Brush::Tool::Resizer );
		mouseResizePos = MouseResizer::BotRight;
	}
	else
	{
		brush.Set( lastTool );
	}
	return false;
}