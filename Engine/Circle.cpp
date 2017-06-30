#include "Circle.h"

void Circle::SetPos( int inX,int inY,int inSize,int inR,int inG,int inB,int inNum )
{
	x = inX;
	y = inY;
	size = inSize;
	R = inR;
	G = inG;
	B = inB;
	num = inNum;
	canDraw = true;
}

void Circle::Draw( Graphics & gfx ) const
{
	if( canDraw )
	{
		for( int i = 0; i < size; ++i )
		{
			for( int j = 0; j < size; ++j )
			{
				if( IsOnScreen( x + j,y + i ) )
					gfx.PutPixel( x + j,y + i,R,G,B );
			}
		}
	}
}

bool Circle::IsOnScreen( int x,int y ) const
{
	if( x <= 0 || x >= Graphics::ScreenWidth ||
		y <= 0 || y >= Graphics::ScreenHeight )
		return false;
	else
		return true;
}
