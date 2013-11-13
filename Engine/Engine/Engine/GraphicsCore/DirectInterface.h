#pragma once
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
//////////////////////////////////////////////////////////////////////////
// DirectInput headers and libraries
//////////////////////////////////////////////////////////////////////////
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <windows.h>
#pragma comment(lib, "DxErr.lib")

#include "GraphicsCore.h"
#include "../Common/LinkList.h"

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


namespace GraphicsCore
{
	class DirectInterface
	{
		private:
			IDirect3D9*			m_pD3DObject;	// Direct3D 9 Object
			IDirect3DDevice9*	m_pD3DDevice;	// Direct3D 9 Device
			D3DCAPS9			m_D3DCaps;		// Device Capabilities
			double mouseScaleX, mouseScaleY;

		public:
			DirectInterface();
			~DirectInterface();

			void Intialize();
			void Shutdown();
			void SetMouseScale();
			double MouseScaleX(){return mouseScaleX;}
			double MouseScaleY(){return mouseScaleY;}
			int ScreenWidth(){return SCREEN_WIDTH;}
			int ScreenHeight(){return SCREEN_HEIGHT;}

			IDirect3DDevice9* GetD3DDevice(){return m_pD3DDevice;}
	};
};