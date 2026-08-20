#pragma once

#include <cstdint>
#include <vector>
#include <Windows.h>

namespace GraphicsUtils
{
	HICON loadIcon(WORD id)
	{
		return static_cast<HICON>(LoadImageW(GetModuleHandle(nullptr), MAKEINTRESOURCEW(id), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	}

	HICON makeGrayscaleIcon(HICON source)
	{
		if (!source)
			return nullptr;

		ICONINFO iconInfo{};
		if (!GetIconInfo(source, &iconInfo))
			return nullptr;

		BITMAP colorBitmap{};
		if (!GetObjectW(iconInfo.hbmColor, sizeof(colorBitmap), &colorBitmap))
		{
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			return nullptr;
		}

		BITMAPINFO bitmapInfo{};
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = colorBitmap.bmWidth;
		bitmapInfo.bmiHeader.biHeight = -colorBitmap.bmHeight; // top-down
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		std::vector<std::uint32_t> pixels(static_cast<std::size_t>(colorBitmap.bmWidth) * colorBitmap.bmHeight);

		HDC dc = GetDC(nullptr);

		if (!GetDIBits(dc, iconInfo.hbmColor, 0, colorBitmap.bmHeight, pixels.data(), &bitmapInfo, DIB_RGB_COLORS))
		{
			ReleaseDC(nullptr, dc);
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			return nullptr;
		}

		for (auto& pixel : pixels)
		{
			const BYTE r = GetRValue(pixel);
			const BYTE g = GetGValue(pixel);
			const BYTE b = GetBValue(pixel);

			const BYTE gray = static_cast<BYTE>(
				0.299 * r +
				0.587 * g +
				0.114 * b
				);

			pixel =
				(pixel & 0xFF000000u) |
				(static_cast<std::uint32_t>(gray) << 16) |
				(static_cast<std::uint32_t>(gray) << 8) |
				gray;
		}

		HBITMAP grayscaleBitmap = CreateDIBitmap(dc, &bitmapInfo.bmiHeader, CBM_INIT, pixels.data(), &bitmapInfo, DIB_RGB_COLORS);

		ReleaseDC(nullptr, dc);

		if (!grayscaleBitmap)
		{
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			return nullptr;
		}

		ICONINFO resultInfo = iconInfo;
		resultInfo.hbmColor = grayscaleBitmap;

		HICON result = CreateIconIndirect(&resultInfo);

		DeleteObject(grayscaleBitmap);
		DeleteObject(iconInfo.hbmColor);
		DeleteObject(iconInfo.hbmMask);

		return result;
	}
}