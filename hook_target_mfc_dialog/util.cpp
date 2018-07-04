#include "stdafx.h"
#include "util.h"

void GdiImageFromResource(Gdiplus::Image **pImg, UINT uImgID, LPCTSTR lpType)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	HRSRC hResInfo = ::FindResource(hInstance, MAKEINTRESOURCE(uImgID), lpType);
	if (hResInfo == NULL)
		return;

	DWORD dwSize;
	dwSize = SizeofResource(hInstance, hResInfo); // get resource size (bytes)
	HGLOBAL hResData;
	hResData = ::LoadResource(hInstance, hResInfo);
	if (hResData == NULL)
		return;

	HGLOBAL hMem;
	hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if (hMem == NULL)
	{
		::FreeResource(hResData);
		return;
	}

	LPVOID lpResData, lpMem;
	lpResData = ::LockResource(hResData);
	lpMem = ::GlobalLock(hMem);
	::CopyMemory(lpMem, lpResData, dwSize); // copy memory
	::GlobalUnlock(hMem);
	::FreeResource(hResData); // free memory

	IStream *pStream;
	HRESULT hr;
	hr = ::CreateStreamOnHGlobal(hMem, TRUE, &pStream); // create stream object
	if (SUCCEEDED(hr))
	{
		*pImg = Gdiplus::Image::FromStream(pStream); // get GDI pointer
		pStream->Release();
	}

	::GlobalFree(hMem);
}
