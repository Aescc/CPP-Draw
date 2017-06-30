#pragma once

#include "Graphics.h"

class Circle
{
public:
	void SetPos( int inX,int inY,int inSize,int inR,int inG,int inB,int inNum );
	void Draw( Graphics& gfx ) const;
	bool IsOnScreen( int x,int y ) const;
// private:
	int x,y,size,R,G,B;
	bool canDraw = false;
	int num;
};