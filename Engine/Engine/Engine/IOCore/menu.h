//#ifndef MENU_H
//#define MENU_H
#pragma once

#include "../Common/observer.h"
#include "../GraphicsCore/GraphicsCore.h"
#include "../GraphicsCore/Sprites.h"
#include "IOCoreMessage.h"
#include <iostream>

namespace IOCore {

class MenuControl : public Observer
{
protected:
	int x, y, w, h;
	int userInfo;

public:
	MenuControl(int ix, int iy, int iw, int ih, int ui=-1):x(ix),y(iy),w(iw),h(ih),userInfo(ui),Observer()	{}
	virtual ~MenuControl(){}
	virtual void Draw(int dx, int dy) = 0;
	virtual bool IsInside(int ix, int iy){ return false; }
	virtual void HandleMouse(MSGData p2){ return; }
	virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
	{
		return false;
	}
	void Send(int to, int type, int qual=0, MSGData p1=0, MSGData p2=0, float delay=0)
		{DISPATCH.Send(ID(), to, type, qual, p1,p2,delay);}
};

class Bar : public MenuControl
{
private:
	int foregroundID;
	int backgroundID;
	int curFill;
	int maxFill;
	int minFill;
	float xscale;
	char text[128];
public:
	Bar(int ix, int iy, int iw, int ih, const char *txt, int fID, int bID, int user) :
				MenuControl(ix, iy, iw, ih, user), foregroundID(fID), backgroundID(bID), maxFill(100), 
				minFill(0), curFill(100), xscale(1)
	{
		if( txt && strlen(txt)>0 )
		{
			strcpy_s(text,128,txt);
		}
		else
		{
			text[0]=0;
		}
	}
	virtual ~Bar(){}

	virtual void Draw(int dx, int dy);
	virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2);
	void UpdateScale();
};

class Text : public MenuControl
{
private:
	int backgroundID;
	char text[128];
public:
	Text(int ix, int iy, int iw, int ih, const char *txt, int bID, int user) :
				MenuControl(ix, iy, iw, ih, user), backgroundID(bID)
	{
		if( txt && strlen(txt)>0 )
		{
			strcpy_s(text,128,txt);
		}
		else
		{
			text[0]=0;
		}
	}
	virtual ~Text(){}

	virtual void Draw(int dx, int dy);
	virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2) { return false; }
};


class Button : public MenuControl
{
private:
	int upID, downID, hoverID;
	bool inHover, isDown;
	char text[128];
public:
	Button(int ix, int iy, int iw, int ih, const char *txt, int usp, int dsp=-1, int hsp=-1, int ui=-1):
				upID(usp),downID(dsp),hoverID(hsp),MenuControl(ix,iy,iw,ih,ui),inHover(false),isDown(false)
	{
		if( txt && strlen(txt)>0 )
		{
			strcpy_s(text,128,txt);
		}
		else
		{
			text[0]=0;
		}
	}
	virtual ~Button(){}

	virtual void Draw(int dx, int dy);
	virtual bool IsInside(int ix, int iy);
	virtual void HandleMouse(MSGData p2);
	//virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2);
};



class Menu : public Observer
{
private:
	GenArray<MenuControl> controlList;
	bool isActive;
	int x, y;
	int w, h;
	int bgID;
	int userInfo;
	bool isInside;

public:
	Menu(int ix, int iy, int iw, int ih, int bground, int ui=-1):
				x(ix),y(iy),w(iw),h(ih),isActive(true),userInfo(ui),isInside(false),Observer()
				{}
	virtual ~Menu(){}
	void Draw();
	int AddControl(MenuControl*);
	void Activate() { isActive = true; }
	void Deactivate() { isActive = false; }
	virtual bool Notify(int, int, int, int, MSGData, MSGData);
};

class MenuManager
{
private:
	GenArray<Menu> menuList;

public:
	MenuManager(){}
	~MenuManager(){}

	int AddMenu(Menu *m);

	void ActivateMenu(int id);
	void DeactivateMenu(int id);
	void Draw();
	void Process() ;
	int CreateMenu(int x, int y, int w, int h, int spriteId, int user);
	int CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int user);
	int CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo);
	int CreateTimer(int menuID, int x, int y, int w, int h, std::string text, int bID,int userInfo);
	int CreateText(int menuID, int x, int y, int w, int h, std::string text, int bID,int userInfo);
	void Flush();
};
};
//#endif