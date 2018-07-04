#pragma once

#include <GdiPlus.h>
#include "util.h"

class gdi_image_render
{
public:
	gdi_image_render(void);
	~gdi_image_render(void);

public:
	BOOL SetImage(UINT nID, LPCTSTR sTR);
	void Render(HDC hdc, Gdiplus::Point pt, const Gdiplus::ImageAttributes *attr = NULL);
	void DrawImg(Gdiplus::Image *pImg, HDC hdc, Gdiplus::Point pt, const Gdiplus::ImageAttributes *attr = NULL);

private:
	Gdiplus::Rect m_DrawRect;
	Gdiplus::Image *m_pImage;
};

