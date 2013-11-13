#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "..\Common\LinkList.h"

#pragma once

namespace GraphicsCore
{
	class Text
	{
	public:
		ID3DXFont*			m_pD3DFont;		// Font Object

		//RECT rect;
		//RECT* srcRect;
		//RECT* dest_Rect;

		void Intialize();

		void DrawText(LPCSTR text, int x, int y, int w, int h, int a, int r, int g, int b);
	};
};