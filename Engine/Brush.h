#pragma once

#include "Graphics.h"

class Brush
{
public:
	enum class Tool
	{
		Brush,
		Eraser,
		Identifier,
		Resizer
	};
public:
	void Set( Tool tool );
	Tool CurTool() const;
private:
	Tool curTool = Tool::Brush;
};