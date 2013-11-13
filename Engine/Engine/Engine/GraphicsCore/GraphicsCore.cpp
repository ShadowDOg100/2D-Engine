#include <windows.h>
#include "GraphicsCore.h"
#include "DirectInterface.h"
#include "Sprites.h"
#include "Text.h"


#define WINDOW_TITLE "GSP 420 - Game Engine"

namespace GraphicsCore
{
	bool screenActive = false;
	DirectInterface DI;
	Sprites spriteManager;
	Text textManager;

	int backgroundSpriteID = -1;
	int worldWidth = 800;
	int worldHeight = 600;

	int camX=0, camY=0;
	int camMinX = 400;
	int camMaxX = 400;
	int camMinY = 300;
	int camMaxY = 300;
	bool useCamera = false;

	HWND hWnd;
	HINSTANCE hinst;

	//Set inRender to default TRUE
	bool inRender = false;

	HWND gethWnd(){return hWnd;}

	HINSTANCE gethInstance(){ return hinst;}
	D3DPRESENT_PARAMETERS D3Dpp;

	LRESULT CALLBACK WndProc(HWND hWnd,			// Handle to the window
						 UINT message,		// Incoming Message
						 WPARAM wparam,		// Message Info
						 LPARAM lparam);	// Message Info

	void InitWindow(void)
	{
		WNDCLASSEX wndClass;  
		ZeroMemory(&wndClass, sizeof(wndClass));

		// set up the window
		wndClass.cbSize			= sizeof(WNDCLASSEX);			// size of window structure
		wndClass.lpfnWndProc	= (WNDPROC)WndProc;				// message callback
		wndClass.lpszClassName	= WINDOW_TITLE;					// class name
		wndClass.hInstance		= hinst;					// handle to the application
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// default cursor
		wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// background brush

		// register a new type of window
		RegisterClassEx(&wndClass);

		hWnd = CreateWindow(
			WINDOW_TITLE, WINDOW_TITLE, 							// window class name and title
			true ? WS_OVERLAPPEDWINDOW | WS_VISIBLE:(WS_POPUP | WS_VISIBLE),// window style
			CW_USEDEFAULT, CW_USEDEFAULT,							// x and y coordinates
			SCREEN_WIDTH, SCREEN_HEIGHT,							// width and height of window
			NULL, NULL,												// parent window and menu
			hinst,											// handle to application
			NULL);

		// Display the window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		screenActive = true;
	}

	void Initialize(HINSTANCE& hInst)
	{
		hinst = hInst;
		InitWindow();
		// Find the width and height of window using hWnd and GetWindowRect()
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		// Set D3D Device presentation parameters before creating the device
		ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

		D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
		D3Dpp.Windowed						= true;								// Windowed or Full-screen boolean
		D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
		D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
		D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
		D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
		D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
		D3Dpp.BackBufferWidth				= width;									// (Same as above)
		D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
		D3Dpp.PresentationInterval			= D3DPRESENT_INTERVAL_ONE; // Present back-buffer immediately, unless V-Sync is on								
		D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
		D3Dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
		D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
		D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

		DI.Intialize();
		spriteManager.Initialize();
		textManager.Intialize();

	}

	int AddSprite(LPCSTR file, int width, int height, int left, int top, int frames, bool loop)
	{
		return spriteManager.CreateSprite(file,width,height,left,top,frames,loop);
	}
	
	///////////////////////
	// CAMERA STUFF
	void FixCameraPosition()
	{
		if( camX > camMaxX )
			camX = camMaxX;
		if( camX < camMinX )
			camX = camMinX;
		if( camY > camMaxY )
			camY = camMaxY;
		if( camY < camMinY )
			camY = camMinY;
	}

	void GetCameraPosition(int &x, int &y)
	{
		x = camX;
		y = camY;
	}

	void MoveCameraTo(int x, int y)
	{
		camX = x;
		camY = y;
		FixCameraPosition();
	}

	void MoveCamera( int dx, int dy)
	{
		camX += dx;
		camY += dy;
		FixCameraPosition();
	}

	void GetCameraOffset(int &x, int &y)
	{
		if( !useCamera ) 
		{
			x = 0; 
			y = 0;
		}
		else
		{
			x = camX - camMinX;
			y = camY - camMinY;
		}
	}
	void UseCamera(bool stat)
	{
		useCamera = stat;
	}

	void SetBackground(int ID, int width, int height) 
	{
		backgroundSpriteID = ID;
		worldWidth = width;
		worldHeight = height;
		camMinX = DI.ScreenWidth()/2;
		camMaxX = width - camMinX;
		camMinY = DI.ScreenHeight()/2;
		camMaxY = height - camMinY;
		FixCameraPosition();
	}



	//////////////////////////////

	void StartDraw()
	{
		// If the device was not created successfully, return
		if(!DI.GetD3DDevice())
		{
			inRender=false;
			return;
		}

		// Clear the back buffer, call BeginScene()
		if(SUCCEEDED(DI.GetD3DDevice()->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0)))
		{
			if(SUCCEEDED(DI.GetD3DDevice()->BeginScene()))
			{
				inRender=true;
				// DRAW THE BACKGROUND
				if( backgroundSpriteID >= 0 )
				{
					useCamera=true;
					DrawSprite(backgroundSpriteID,worldWidth,worldHeight,0,0);
					useCamera=false;
				}
				return;
			}
		}
		inRender = false;
		return;
	}

	void DrawSprite(int ID, int width, int height, int x, int y)
	{
		if(!inRender) return;

		spriteManager.Draw(ID, width, height, x, y);
	}

	void DrawText(LPCSTR text, int x, int y, int w, int h, int a, int r, int g, int b)
	{
		textManager.DrawText(text, x, y, w, h, a, r, b, g);
	}

	void FinishDraw()
	{
		if( inRender )
		{
			DI.GetD3DDevice()->EndScene();
			DI.GetD3DDevice()->Present(0, 0, 0, 0);
			inRender = false;
		}
	}
	double MouseScaleX()
	{
		return DI.MouseScaleX();
	}
	double MouseScaleY()
	{
		return DI.MouseScaleY();
	}

	void Shutdown()
	{
		DI.Shutdown();
		spriteManager.shutdown();
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch(message)
	{
		case WM_SIZE:
		{
			DI.SetMouseScale();
		}
		case (WM_PAINT):
		{
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}		
		case(WM_DESTROY):
		{
			screenActive=false;
			PostQuitMessage(0); 
			break;
		}
		case(WM_KEYDOWN):
		{
			switch(wparam)
			{
				// Escape Key will close the application
				// Remove/Modify this call when creating a real game with menus, etc.
				case VK_ESCAPE:
				{
				screenActive=false;
					PostQuitMessage(0);
					break;
				}
			}
		}
	}

	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}
	bool ActiveScreen()
	{
		return screenActive;
	}
	void ClearSprites()
	{
		spriteManager.ClearSprites();
	}
};