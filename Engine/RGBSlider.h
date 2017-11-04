#pragma once

#include "Graphics.h"
#include "Mouse.h"

class RGBSlider
{
private:
	class Slider
	{
	public:
		Slider( int x,int y,Color c );
		bool HandleClick( Mouse& ms );
		void Draw( Graphics& gfx ) const;
		int ColorAmount() const
		{
			return activeColor;
		}
	public:
		const int x;
		const int y;
		const Color c;
		static constexpr int width = 12;
		static constexpr int height = 85;
		int activeColor = 255;
	};
public:
	bool HandleClick( Mouse& ms );
	void Draw( Graphics& gfx ) const;
	Color CheckColor() const
	{
		return Color( red.ColorAmount(),grn.ColorAmount(),blu.ColorAmount() );
	}
private:
	Slider red = { 0,20,Colors::Red };
	Slider grn = { int( float( Slider::width ) * 1.5f ),20,Colors::Green };
	Slider blu = { int( float( Slider::width ) * 3.0f ),20,Colors::Blue };
};