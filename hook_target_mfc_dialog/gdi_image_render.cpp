#include "StdAfx.h"
#include "gdi_image_render.h"


gdi_image_render::gdi_image_render(void)
{
	m_DrawRect = Gdiplus::Rect(0, 0, 0, 0);
}


gdi_image_render::~gdi_image_render(void)
{
}

BOOL gdi_image_render::SetImage(UINT nID, LPCTSTR sTR)
{
	GdiImageFromResource(&m_pImage, nID, sTR);
	if (m_pImage)
		m_DrawRect = Gdiplus::Rect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	return TRUE;
}

void gdi_image_render::Render(HDC hdc, Gdiplus::Point pt, const Gdiplus::ImageAttributes *attr /*= NULL*/)
{
	DrawImg(m_pImage, hdc, pt, attr);
}

void gdi_image_render::DrawImg(Gdiplus::Image *pImg, HDC hdc, Gdiplus::Point pt, const Gdiplus::ImageAttributes *attr /*= NULL*/)
{
	Gdiplus::Graphics graph(hdc);
	if (pImg)
	{
		Gdiplus::Rect crt(pt.X, pt.Y, m_DrawRect.Width, m_DrawRect.Height);
		graph.DrawImage(pImg, crt, m_DrawRect.GetLeft(), m_DrawRect.GetTop(), m_DrawRect.Width, m_DrawRect.Height, Gdiplus::UnitPixel);
	}
}
