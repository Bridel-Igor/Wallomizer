#pragma once

#include <Windows.h>

namespace GraphicsUtils
{
	HICON loadIcon(WORD id);
	HICON makeGrayscaleIcon(HICON source);
}
