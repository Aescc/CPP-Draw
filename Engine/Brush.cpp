#include "Brush.h"

void Brush::Set( Tool tool )
{
	curTool = tool;
}

Brush::Tool Brush::CurTool() const
{
	return curTool;
}
