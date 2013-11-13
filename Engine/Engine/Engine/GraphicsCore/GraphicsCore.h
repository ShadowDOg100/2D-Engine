#pragma once
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

namespace GraphicsCore
{
	void InitWindow(void);

	void Initialize(HINSTANCE& hInst); //Init Hinstance for group.

	int AddSprite(LPCSTR file, int width, int height, int left, int top, int frames, bool loop);

	void SetBackground(int ID, int width, int height);

	void StartDraw();

	void DrawSprite(int ID, int width, int height, int x, int y);
	void DrawText(LPCSTR text, int x, int y, int w, int h, int a, int r, int b, int g);

	void FinishDraw();
	
	void Shutdown();	//shutdown

	HWND gethWnd();
	HINSTANCE gethInstance();
	bool ActiveScreen();
	double MouseScaleX();
	double MouseScaleY();
	void GetCameraPosition(int &x, int &y);
	void MoveCameraTo(int x, int y);
	void MoveCamera( int dx, int dy);
	void GetCameraOffset(int &x, int &y);
	void UseCamera(bool stat);
	void ClearSprites();

};