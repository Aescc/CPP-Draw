#pragma once

#include "Graphics.h"

class Brush
{
public:
	void SetBrush();
	void SetEraser();
	void SetMover();
	void SetIdentify();
	bool GetBrush();
	bool GetEraser();
	bool GetMover();
	bool GetIdentify();
private:
	void SetAllFalse();
private:
	bool brush = true;
	bool eraser = false;
	bool mover = false;
	bool identify = false;
};