#include "Overlay.h"

void Overlay::Draw( Graphics& gfx ) const
{
	Rect( 0,0,left,Graphics::ScreenHeight,gfx,70,70,70 );
	Rect( 0,0,Graphics::ScreenWidth,top,gfx,70,70,70 );
	Rect( 0,Graphics::ScreenHeight - bot,Graphics::ScreenWidth,Graphics::ScreenHeight,gfx,70,70,70 );
	Rect( Graphics::ScreenWidth - right,0,Graphics::ScreenWidth + right,Graphics::ScreenHeight,gfx,70,70,70 );
	Rect( 0,0,100,100,gfx,R,G,B );
}

void Overlay::Rect( int x0,int y0,int x1,int y1,Graphics& gfx,int nR,int nG,int nB ) const
{
	for( int i = 0; i + y0 < y1; ++i )
	{
		for( int j = 0; j + x0 < x1; ++j )
		{
			gfx.PutPixel( x0 + j,y0 + i,nR,nG,nB );
		}
	}
}

void Overlay::UpdateColor( int newR,int newG,int newB )
{
	R = newR;
	G = newG;
	B = newB;
}

void Overlay::DrawMouse( int x,int y,int size,int iR,int iG,int iB,Graphics& gfx ) const
{
	for( int i = 0; i < size; ++i )
	{
		if( IsOnScreen( x + i - size / 2,y - size / 2 ) )
			gfx.PutPixel( x + i - size / 2,y - size / 2,iR,iG,iB );
		if( IsOnScreen( x + i - size / 2,y + size / 2 ) )
			gfx.PutPixel( x + i - size / 2,y + size / 2,iR,iG,iB );
		if( IsOnScreen( x - size / 2,y + i - size / 2 ) )
			gfx.PutPixel( x - size / 2,y + i - size / 2,iR,iG,iB );
		if( IsOnScreen( x + size / 2,y + i - size / 2 ) )
			gfx.PutPixel( x + size / 2,y + i - size / 2,iR,iG,iB );
	}
}

bool Overlay::IsOnScreen( int x,int y ) const
{
	if( x <= 0 || x >= Graphics::ScreenWidth ||
		y <= 0 || y >= Graphics::ScreenHeight )
		return false;
	else
		return true;
}

int Overlay::Top() const
{
	return top;
}

int Overlay::Bot() const
{
	return Graphics::ScreenHeight - bot;
}

int Overlay::Left() const
{
	return left;
}

int Overlay::Right() const
{
	return Graphics::ScreenWidth - right;
}