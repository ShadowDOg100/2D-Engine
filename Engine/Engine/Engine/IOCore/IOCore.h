#ifndef IOCORE_H_ 
#define IOCORE_H_ 
#pragma once
#include <fstream>
#include <string>

namespace IOCore  
{ 	
	void Initialize(); 
	void Shutdown(); 
	void Draw(); 
	void Process();
	int CreateMenu(int x, int y, int w, int h, int spriteId, int user);
	//int CreateControl(int ID, int type, int x, int y, int w, int h, std::string text, int *sIDs, int UserInfo);
	int CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int UserInfo);
	int CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo);
	int CreateTimer(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo);
	int CreateText(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo);
	void ClearLevel();
}; 

#endif 
