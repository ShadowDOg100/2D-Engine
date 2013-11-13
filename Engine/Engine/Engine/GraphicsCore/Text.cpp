#include"GraphicsCore.h"
#include "DirectInterface.h"
#include "Text.h"

namespace GraphicsCore
{
	extern DirectInterface DI;

	void Text::Intialize()
	{
		//////////////////////////////////////////////////////////////////////////
		// Create a Font Object
		//////////////////////////////////////////////////////////////////////////
	
		// Load a font for private use for this process
		AddFontResourceEx("Delicious-Roman.otf", FR_PRIVATE, 0);

		// Load D3DXFont, each font style you want to support will need an ID3DXFont
		D3DXCreateFont(DI.GetD3DDevice(), 20, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Delicious-Roman"), &m_pD3DFont);
	}

	void Text::DrawText(LPCSTR text, int x, int y, int w, int h, int a, int r, int g, int b)
	{
		HWND hWnd = gethWnd();
		int camOffX=0, camOffY=0;
		GetCameraOffset(camOffX,camOffY);
		x-=camOffX;
		y-=camOffY;

		//////////////////////////////////////////////////////////////////////////
		// Draw Text
		//////////////////////////////////////////////////////////////////////////

		// Calculate RECT structure for text drawing placement, using whole screen
		RECT rect;
		//GetWindowRect(hWnd, &rect);
		rect.right = x+w;
		rect.bottom = y+h;
		rect.left = x;
		rect.top = y;
		
		// Draw Text, using DT_TOP, DT_RIGHT for placement in the top right of the
		// screen.  DT_NOCLIP can improve speed of text rendering, but allows text
		// to be drawn outside of the rect specified to draw text in.
		m_pD3DFont->DrawText(0, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		//DrawText(0, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
};