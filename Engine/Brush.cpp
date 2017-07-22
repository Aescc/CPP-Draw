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

bool Brush::GetBrush()
{
	return brush;
}

bool Brush::GetEraser()
{
	return eraser;
}

bool Brush::GetMover()
{
	return mover;
}

bool Brush::GetIdentify()
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
