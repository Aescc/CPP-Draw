#include "Brush.h"

void Brush::SetBrush()
{
	SetAllFalse();
	brush = true;
}

void Brush::SetEraser()
{
	SetAllFalse();
	eraser = true;
}

void Brush::SetMover()
{
	SetAllFalse();
	mover = true;
}

void Brush::SetIdentify()
{
	SetAllFalse();
	identify = true;
}

bool Brush::GetBrush() const
{
	return brush;
}

bool Brush::GetEraser() const
{
	return eraser;
}

bool Brush::GetMover() const
{
	return mover;
}

bool Brush::GetIdentify() const
{
	return identify;
}

void Brush::SetAllFalse()
{
	bool brush = false;
	bool eraser = false;
	bool mover = false;
	bool identify = false;
}
