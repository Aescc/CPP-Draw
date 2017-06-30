#pragma once

#include "Graphics.h"

class Overlay
{
public:
	void Draw( Graphics& gfx ) const;
	void Rect( int x0,int y0,int x1,int y1,Graphics& gfx,int nR,int nG,int nB ) const;
	void UpdateColor( int R,int G,int B );
	void DrawMouse( int x,int y,int size,int iR,int iG,int iB,Graphics& gfx ) const;
	bool IsOnScreen( int x,int y ) const;
	int Top() const;
	int Bot() const;
	int Left() const;
	int Right() const;
private:
	static constexpr int top = 20;
	static constexpr int bot = 20;
	static constexpr int left = 100;
	static constexpr int right = 20;
	int R,G,B;
};