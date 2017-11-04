#include "RGBSlider.h"

RGBSlider::Slider::Slider( int x,int y,Color c )
	:
	x( x ),
	y( y ),
	c( c )
{
}

bool RGBSlider::Slider::HandleClick( Mouse& ms )
{
	if( ms.GetPosX() >= x && ms.GetPosX() < x + width &&
		ms.GetPosY() >= y && ms.GetPosY() - y < height )
	{
		const int posClicked = ms.GetPosY() - y;

		activeColor = posClicked * 3;
		return true;
	}
	return false;
}

void RGBSlider::Slider::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( x,y,width,height,c );
	Color nextColor;
	if( c.GetR() == 255 )
	{
		nextColor = { 155,10,10 };
	}
	else if( c.GetG() == 255 )
	{
		nextColor = { 10,155,10 };
	}
	else if( c.GetB() == 255 )
	{
		nextColor = { 10,10,155 };
	}
	gfx.DrawRect( x,y + activeColor / 3,width,5,nextColor );
}

bool RGBSlider::HandleClick( Mouse& ms )
{
	return( red.HandleClick( ms ) ||
		grn.HandleClick( ms ) ||
		blu.HandleClick( ms ) );
}

void RGBSlider::Draw( Graphics& gfx ) const
{
	red.Draw( gfx );
	grn.Draw( gfx );
	blu.Draw( gfx );
}
