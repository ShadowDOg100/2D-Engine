#include "../Common/Observer.h"
#include "InputManager.h"
#include "menu.h"

namespace IOCore {

	class IOManager : public Observer 
	{ 
	private:
		static IOManager instance;
		IOManager(); 
		InputManager* inputManager;
		MenuManager* menuManager;
	public:
		static IOManager* getInstance();
		virtual ~IOManager(); 
		//virtual bool Notify(int to, int from, int type, int qual, MSGData p1, MSGData p2){return false;} 
		void Initialize();
		void Shutdown();
		void Draw();
		void Process();
		int CreateMenu(int x, int y, int w, int h, int spriteId, int user);
		int CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int UserInfo);
		int CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo);
		int CreateTimer(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo);
		int CreateText(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo);
		MenuManager* getMenuManager() { return menuManager; }
	}; 
};