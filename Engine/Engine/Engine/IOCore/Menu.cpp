#include "InputManager.h"
#include "..\GraphicsCore\GraphicsCore.h"
#include "../MainCore/MainCoreMessages.h"
#include "IOManager.h"
#include "IOCoreMessage.h"
#include "menu.h"
#include "TimerControl.h"
#include "IOCore.h"

namespace IOCore
{
	void Menu::Draw()
	{
		if( !isActive ) return;
		if( bgID >= 0 )
			GraphicsCore::DrawSprite(bgID,w,h,x,y);
		for( int i=0; i< controlList.Top(); i++ )
		{
			if( controlList[i] )
				controlList[i]->Draw(x,y);
		}
	}
	int Menu::AddControl(MenuControl* mc) 
	{
		return controlList.Add(mc);
	}
	bool Menu::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
	{ 
		switch(type)
		{
		case IO_T_MOUSE:
			{
				switch( qual )
				{
				case IO_Q_MOUSEMOVE:
					{
						int mx = p2.w.lo-x; // position translated to menu relative
						int my = p2.w.hi-y;
						// assume that p1 contains mouse position in p1.w.lo and p1.w.hi
						if( mx >= 0 && mx <= w &&
							my >= 0 && my <= h ) // the mouse is withing the menu
						{
							isInside=false;
							for(int i=0; i<controlList.Top(); i++ )
							{
								if( controlList[i] )
								{
									if( controlList[i]->IsInside(mx, my) )
									{
										controlList[i]->HandleMouse(p1);
									}
								}
							}
							return true;
						}
						else
						{
							if( isInside )
							{
								for(int i=0; i<controlList.Top(); i++ )
								{
									if( controlList[i] )
									{
										controlList[i]->IsInside(-1, -1);
									}
								}
								isInside=false;
							}
							return false;
						}
					}
				}
			}
		}
		return false;
	}

	void Button::Draw(int dx, int dy)
	{
		int useID = upID;
		if( isDown )
			useID = downID;
		else if( inHover )
			useID = hoverID;

		GraphicsCore::DrawSprite(useID,w,h,x+dx,y+dy);
		if( text[0] != 0 )
			GraphicsCore::DrawText(text,x+dx,y+dy,w,h,0,255,255,255);

	}
	bool Button::IsInside(int ix, int iy)
	{
		if( ix >= x && ix <= x+w && iy >= y && iy <= y+h )
			return true;
		inHover = false;
		isDown = false;
		return false;
	}
	void Button::HandleMouse(MSGData bs)
	{
		// bs.b.b1 contains bit flags for mouse buttons 0-7
		// bs.b.b2 contains state of shift key
		// bs.b.b3 contains state of ctrl key
		// bs.b.b4 contains state of alt key

		bool down = bs.b.b1&1;
		if( !down )
		{
			if( isDown ) // mouse button was just release on the the button
			{			// Send a click message for this button
				MSGData p1, p2;
				p1.i = ID();
				p2.i = userInfo;
				Send(-1,IO_T_MENU_BUTTON, IO_Q_BUTTON_CLICK, p1, p2, 0);
				isDown = false;
				inHover = true;
			} 
			else
			{
				inHover = true;
			}
		}
		else
		{
			isDown = true;
			inHover = false;
		}
	}

	//bool Button::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2) {
	//	if(type == IO_T_MOUSE) {
	//		bool isInside = IsInside(p2.w.lo, p2.w.hi);
	//		if(isInside) {
	//			HandleMouse(p1);
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	int MenuManager::AddMenu(Menu *m)
	{
		return menuList.Add(m);
	}

	void MenuManager::ActivateMenu(int id)
	{
		if( menuList[id] )
			menuList[id]->Activate();
	}

	void MenuManager::DeactivateMenu(int id)
	{
		if( menuList[id] )
			menuList[id]->Deactivate();
	}

	void MenuManager::Draw()
	{
		for( int i=0; i<menuList.Top(); i++ )
		{
			if( menuList[i] )
				menuList[i]->Draw();
		}
	}

	void MenuManager::Process() 
	{

	}

	int MenuManager::CreateMenu(int x, int y, int w, int h, int spriteId, int user)
	{
		return AddMenu(new Menu(x, y, w, h, spriteId, user));
	}
	int MenuManager::CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int user)
	{
		if(menuList[ID])
			return menuList[ID]->AddControl(new Button(x, y, w, h, text.c_str(), Usp, Dsp, Hsp, user));
		return -1;
	}
	int MenuManager::CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo) 
	{
		if(menuList[ID])
			return menuList[ID]->AddControl(new Bar(x, y, w, h, text.c_str(), fID, bID, UserInfo));
		return -1;
	}
	int MenuManager::CreateTimer(int menuID, int x, int y, int w, int h, std::string text, int bID,int userInfo)
	{
		if( menuList[menuID])
			return menuList[menuID]->AddControl(new Timer(x,y,w,h,text.c_str(),bID,userInfo));
		return -1;
	}
	int MenuManager::CreateText(int menuID, int x, int y, int w, int h, std::string text, int bID,int userInfo)
	{
		if( menuList[menuID])
			return menuList[menuID]->AddControl(new Text(x,y,w,h,text.c_str(),bID,userInfo));
		return -1;
	}


	void MenuManager::Flush() {
		menuList.Flush();
	}

	void Bar::Draw(int dx, int dy)
	{
		if(backgroundID >= 0)
			GraphicsCore::DrawSprite(backgroundID, w, h, x+dx, y+dy);
		if(foregroundID >= 0 && xscale > 0) {
			int tempx = x+dx+2;
			int tempy = y+dy+2;
			int tempw = w - 4;
			int temph = h - 4;
			GraphicsCore::DrawSprite(foregroundID, (int)(tempw*xscale), temph, tempx, tempy);
		}
		if( strlen(text) > 0 )
		{
			char str[128];
			sprintf_s(str,128,text,xscale*100);
			GraphicsCore::DrawText(str,x+dx,y+dy,w,h,255,255,255,255);
		}


	}
	bool Bar::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
	{
		switch(type) 
		{
		case MC_T_HUDCHANGE:
			{
				if(p1.i != userInfo) return false;
				switch(qual)
				{
				case MC_Q_SET:
					{
						curFill = p2.i;
						UpdateScale();
						return true;
					}
				case MC_Q_INCREMENT:
					{
						curFill += p2.i;
						UpdateScale();
						MSGData p1t, p2t;
						p1t.i = curFill;
						p2t.i = userInfo;
						Send(-1,IO_T_CONTROLCHANGE,-1,p1t,p2t,0);
						return true;
					}
				case MC_Q_INQUIRE:
					{
						MSGData p1t, p2t;
						p1t.i = curFill;
						p2t.i = userInfo;
						Send(-1,IO_T_CONTROLCHANGE,-1,p1t,p2t,0);
						return true;
					}
				case MC_Q_SETRANGE:
					{
						minFill = p2.w.lo;
						maxFill = p2.w.hi;
						UpdateScale();
						return true;
					}
				}
			}
			break;
		}
		return false;
	}
	void Bar::UpdateScale() {
		if(curFill < minFill)
			curFill = minFill;
		if(curFill > maxFill)
			curFill = maxFill;
		xscale = (curFill-minFill)/(float)(maxFill-minFill);
	}

	void Text::Draw(int dx, int dy) {
		if(backgroundID >= 0)
			GraphicsCore::DrawSprite(backgroundID, w, h, x+dx, y+dy);

		char str[128];
		if( strlen(text) > 0 )
		{
			sprintf_s(str,128,text);
		}
		GraphicsCore::DrawText(str,x+dx,y+dy,w,h,255,255,255,255);
	}
};



	


