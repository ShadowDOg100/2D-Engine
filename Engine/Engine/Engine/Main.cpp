#include <iostream>
using namespace std;
#include <windows.h>
#define VC_EXTRALEAN

#include "MainCore\MainCore.h"
#include "GraphicsCore\GraphicsCore.h"

// Entry point for the game or application.
//int WINAPI wWinMain(HINSTANCE hInstance,	// Handle to the application
//				   HINSTANCE hPrevInstance,	// Handle to the previous app
//				   LPTSTR lpCmdLine,		// Command line string
//				   int lpCmdShow);			// Show window value

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{

	// Initialize DirectX/Game here
	MainCore::Initialize(hInstance);
	MainCore::LoadGame("game1");
	
	// Use this msg structure to catch window messages
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));

	// Main Windows/Game Loop
	while(msg.message != WM_QUIT && GraphicsCore::ActiveScreen())
	{
		if(PeekMessage(&msg, GraphicsCore::gethWnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// This is where you call your DirectX/Game render/update calls
		MainCore::Process();


	}

	//Shutdown DirectX/Game here
	MainCore::Shutdown();

	// Return successful
	return 0;
}
