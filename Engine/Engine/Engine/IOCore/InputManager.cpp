//#include "..\common\observer.h"
#include "InputManager.h"
#include "IOCoreMessage.h"
#include "../GraphicsCore/GraphicsCore.h"

//#include <dinput.h>

#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

InputManager::InputManager(HWND hWnd, HINSTANCE hInst):Observer()
{
        DirectInput8Create(GraphicsCore::gethInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);

        // Create Direct Input Devices
        m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
        m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

        // Set Data Format
        m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
        m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

        // Set Cooperative Level
        m_pDIKeyboard->SetCooperativeLevel(GraphicsCore::gethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        m_pDIMouse->SetCooperativeLevel(GraphicsCore::gethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		
        m_pDIKeyboard->Acquire();
        m_pDIMouse->Acquire();
        m_pDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		//keyboard data
        for (int i = 0; i < 256; i++) {
                buffer[i] = false;
        }

		//mouse data
		for (int j = 0; j < 8; j++){
				mouseState[j] = false;
		}

		//initialize mouse (x,y) vars
		POINT p;
		GetCursorPos(&p);
		m_mouseX = p.x;
		m_mouseY = p.y;
}

InputManager::~InputManager() {
        SAFE_RELEASE(m_pDIObject);

        if(m_pDIKeyboard != NULL)
        {
                m_pDIKeyboard->Unacquire();
                SAFE_RELEASE(m_pDIKeyboard);
        }

        if(m_pDIMouse != NULL)
        {
                m_pDIMouse->Unacquire();
                SAFE_RELEASE(m_pDIMouse);
        }
}


void InputManager::Process() {

		//Process keyboard data
        char tempBuffer[256];
        m_pDIKeyboard->Acquire();
        m_pDIMouse->Acquire();
        m_pDIKeyboard->GetDeviceState(sizeof(buffer), &tempBuffer);

        for(int i = 0; i < 256; i++) {
			if(tempBuffer[i] == -52) 
				continue;
			bool temp = (tempBuffer[i] & 0x80)?true:false;
            if(temp != buffer[i]) {
                int qual = 0;
                if(buffer[i]) {
                    qual = IO_Q_KEYRELEASE;
                } else {
                    qual = IO_Q_KEYDOWN;
                }

                MSGData p1, p2;

				p1.i = i;
				p2.b.b1 = tempBuffer[DIK_LCONTROL] || tempBuffer[DIK_RCONTROL];
				p2.b.b2 = tempBuffer[DIK_LSHIFT] || tempBuffer[DIK_RSHIFT];
				p2.b.b3 = tempBuffer[DIK_LALT] || tempBuffer[DIK_RALT];


                SendObserverMessage(-1, IO_T_KEYBOARD, qual, p1, p2,0.0);

                buffer[i] = (tempBuffer[i] & 0x80)?true:false;
            }
        }


		//Process mouse data
		DIMOUSESTATE2 tempMSBuffer[8];
		bool tempmousestate[8];
		for(int i = 0; i < 8; i++) {
			tempmousestate[i] = false;
		}
        m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &tempMSBuffer);
		
		bool buttonMsg = false;
		bool mouseMoveMsg = false;
        for(int i = 0; i < 8; i++) 
		{
			//Process mouse-click data
			bool temp = (tempMSBuffer->rgbButtons[i] & 0x80)?true:false;
			tempmousestate[i] = temp;
			if(temp != mouseState[i]) 
			{
				buttonMsg = true;
				mouseState[i] = tempmousestate[i];
            }
        }

		//Process mouse movement data
		if (ms.lX != 0 || ms.lY != 0)
		{
			mouseMoveMsg = true;
			POINT p;
			GetCursorPos(&p);
			//convert p from screen coords, to client coords
			HWND hwnd = GraphicsCore::gethWnd();
			ScreenToClient(hwnd, &p);
			//set variables for mouse(x,y)
			m_mouseX = (int)(p.x*GraphicsCore::MouseScaleX());
			m_mouseY = (int)(p.y*GraphicsCore::MouseScaleY());
		}

		if( mouseMoveMsg || buttonMsg )
		{
			MSGData p1, p2;
			p1.b.b1 = ((tempmousestate[0])?1:0) +
					((tempmousestate[1])?2:0) +
					((tempmousestate[2])?4:0) +
					((tempmousestate[3])?8:0) +
					((tempmousestate[4])?16:0) +
					((tempmousestate[5])?32:0) +
					((tempmousestate[6])?64:0) +
					((tempmousestate[7])?128:0);
			p1.b.b2 = tempBuffer[DIK_LCONTROL] || tempBuffer[DIK_RCONTROL];
			p1.b.b3 = tempBuffer[DIK_LSHIFT] || tempBuffer[DIK_RSHIFT];
            p1.b.b4 = tempBuffer[DIK_LALT] || tempBuffer[DIK_RALT];
			p2.w.lo = m_mouseX;
			p2.w.hi = m_mouseY;
			if( mouseMoveMsg )
				SendObserverMessage(-1, IO_T_MOUSE, IO_Q_MOUSEMOVE, p1, p2, 0.0);
			if( buttonMsg )
                SendObserverMessage(-1, IO_T_MOUSE, IO_Q_MBCHANGE, p1, p2, 0.0);
		}

}