/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	// if( wnd.mouse.LeftIsPressed() && wnd.mouse.GetPosX() < Graphics::ScreenWidth - 1 && wnd.mouse.GetPosY() < Graphics::ScreenHeight - 1 )
	if( wnd.mouse.LeftIsPressed() )
	{
		int x = wnd.mouse.GetPosX();
		int y = wnd.mouse.GetPosY();
		if( x > Graphics::ScreenWidth || x < 0 )
			x = 1;
		if( y > Graphics::ScreenHeight || y < 0 )
			y = 1;

		if( x < overlay.Left() )
			x = overlay.Left();
		if( x > overlay.Right() )
			x = overlay.Right();
		if( y < overlay.Top() )
			y = overlay.Top();
		if( y > overlay.Bot() )
			y = overlay.Bot();

		circles[currentCircle].SetPos( x - size / 2,y - size / 2,size,R,G,B,liftCounter );
		++currentCircle;
	}
	else
		++liftCounter;

	if( wnd.kbd.KeyIsPressed( 219 ) && size > 1 )
		--size;
	if( wnd.kbd.KeyIsPressed( 221 ) && size < 100 )
		++size;

	if( wnd.kbd.KeyIsPressed( 82 ) )
	{
		if( wnd.kbd.KeyIsPressed( VK_SHIFT ) )
			--R;
		else
			++R;
	}
	if( wnd.kbd.KeyIsPressed( 71 ) )
	{
		if( wnd.kbd.KeyIsPressed( VK_SHIFT ) )
			--G;
		else
			++G;
	}
	if( wnd.kbd.KeyIsPressed( 66 ) )
	{
		if( wnd.kbd.KeyIsPressed( VK_SHIFT ) )
			--B;
		else
			++B;
	}
	if( R < 0 )
		R = 0;
	if( R > 255 )
		R = 255;
	if( G < 0 )
		G = 0;
	if( G > 255 )
		G = 255;
	if( B < 0 )
		B = 0;
	if( B > 255 )
		B = 255;
	overlay.UpdateColor( R,G,B );
}

void Game::ComposeFrame()
{
	for( int i = 0; i < MAX_CIRCLES; ++i )
	{
		circles[i].Draw( gfx );
		if( circles[i].canDraw && circles[i + 1].canDraw && circles[i].num == circles[i + 1].num &&
			FindDist( circles[i].x,circles[i].y,circles[i + 1].x,circles[i + 1].y ) < gfx.ScreenHeight / 2 )
			DrawLine( circles[i].x,circles[i].y,circles[i + 1].x,circles[i + 1].y,circles[i].R,circles[i].G,circles[i].B,circles[i].size );
	}
	overlay.Draw( gfx );
	overlay.DrawMouse( wnd.mouse.GetPosX(),wnd.mouse.GetPosY(),size,R,G,B,gfx );
}

int Game::FindDist( int x0,int y0,int x1,int y1 ) const
{
	const int deltaX = x1 - x0;
	const int deltaY = y1 - y0;

	const int dist = sqrt( ( deltaX * deltaX ) + ( deltaY * deltaY ) );
	return dist;
}

int Game::FindSlope( int x0,int y0,int x1,int y1 ) const
{
	const int deltaX = x1 - x0;
	const int deltaY = y1 - y0;
	
	if( deltaX == 0 )
		return 0;

	const int slope = deltaY / deltaX;
	return slope;
}

void Game::DrawLine( int x0,int y0,int x1,int y1,int R,int G,int B,int inSize )
{
	//If horizontal line
	if( y0 == y1 )
	{
		//Make sure x1<x2
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
			if( IsOnScreen( i,y0 ) )
				DrawCircle( i,y0,inSize,R,G,B );
		}
		return;
	}

	//If vertical line
	if( x0 == x1 )
	{
		//Make sure y1<y2
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
			if( IsOnScreen( x0,i ) )
				DrawCircle( x0,i,inSize,R,G,B );
		}
		return;
	}

	int distance = FindDist( x0,y0,x1,y1 ) + 1;
	float ySlope = (float)( y1 - y0 ) / (float)distance;
	float xSlope = (float)( x1 - x0 ) / (float)distance;

	for( int i = 0; i < distance; ++i )
	{
		int newX = x0 + i * xSlope + 0.5;
		int newY = y0 + i * ySlope + 0.5;
		if( IsOnScreen( newX,newY ) )
			DrawCircle( newX,newY,inSize,R,G,B );
	}
}

void Game::DrawCircle( int x,int y,int size,int R,int G,int B )
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

bool Game::IsOnScreen( int x,int y ) const
{
	if( x <= 0 || x >= gfx.ScreenWidth ||
		y <= 0 || y >= gfx.ScreenHeight )
		return false;
	else
		return true;
}