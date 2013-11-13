#pragma once

// Direct Input lib
#define DIRECTINPUT_VERSION 0x0800
#include "..\common\observer.h"
#include <dinput.h>
#include "Windows.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


class InputManager : public Observer
{
private:

        // attributes
        IDirectInput8*                  m_pDIObject;		// DirectInput Object
        IDirectInputDevice8*			m_pDIKeyboard;		// Keyboard device
        IDirectInputDevice8*			m_pDIMouse;			// Mouse device
        bool                            buffer[256];		// Stores keyboard state
        bool							mouseState[8];			// Mouse state for 8 button mouse
		int								m_mouseX;			//variable for mouse position X
		int								m_mouseY;			//variable for mouse position Y
		DIMOUSESTATE2					ms;

public:
        // Constructor
        InputManager(HWND hWnd, HINSTANCE hInst);

        // Destructor
        virtual ~InputManager(void);
 
        void Process();
		void SendObserverMessage(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
			{DISPATCH.Send(ID(), to, type, qual, p1,p2,delay);}

		//BOOL ScreenToClient(
		//		_In_ HWND *hWnd,
		//		LPPOINT lpPoint
		//	);

};