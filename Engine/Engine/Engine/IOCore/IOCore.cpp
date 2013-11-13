#include "IOCore.h"
#include "IOManager.h"

namespace IOCore 
{

	void Initialize()
	{
		IOManager::getInstance()->Initialize();
	}

	void Shutdown()
	{
		IOManager::getInstance()->Shutdown();
	}

	void Draw()
	{
		IOManager::getInstance()->Draw();
	}

	void Process()
	{
		IOManager::getInstance()->Process();
	}

	int CreateMenu(int x, int y, int w, int h, int spriteId, int user)
	{
		return IOManager::getInstance()->CreateMenu(x, y, w, h, spriteId, user);
	}

	int CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int UserInfo) 
	{
		return IOManager::getInstance()->CreateButton(ID, x, y, w, h, text, Usp, Hsp, Dsp, UserInfo);
	}

	int CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo)
	{
		return IOManager::getInstance()->CreateBar(ID, x, y, w, h, text, fID, bID, UserInfo);
	}
	int CreateTimer(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo)
	{
		return IOManager::getInstance()->CreateTimer(ID,x,y,w,h,text,bID,userInfo);
	}
	int CreateText(int ID, int x, int y, int w, int h, std::string text, int bID, int userInfo)
	{
		return IOManager::getInstance()->CreateText(ID,x,y,w,h,text,bID,userInfo);
	}

	IOManager::IOManager() : Observer()
	{
		inputManager = NULL;
		menuManager = NULL;
	}

	IOManager::~IOManager() 
	{

	}

	IOManager *IOManager::getInstance()
	{
		static IOManager * instance = new IOManager();
		return instance;
	}

	void IOManager::Initialize() 
	{
		inputManager = new InputManager(GraphicsCore::gethWnd(), GraphicsCore::gethInstance());
		menuManager = new MenuManager();
	}

	void IOManager::Shutdown() 
	{
		delete inputManager;
		delete menuManager;
	}

	void IOManager::Draw() 
	{
		menuManager->Draw();
	}

	void IOManager::Process() 
	{
		inputManager->Process();
		menuManager->Process();
	}

	int IOManager::CreateMenu(int x, int y, int w, int h, int spriteId, int user) 
	{
		return menuManager->CreateMenu(x, y, w, h, spriteId, user);
	}

	int IOManager::CreateButton(int ID, int x, int y, int w, int h, std::string text, int Usp, int Hsp, int Dsp, int UserInfo) 
	{
		return menuManager->CreateButton(ID, x, y, w, h, text, Usp, Hsp, Dsp, UserInfo);
	}

	int IOManager::CreateBar(int ID, int x, int y, int w, int h, std::string text, int fID, int bID, int UserInfo) 
	{
		return menuManager->CreateBar(ID, x, y, w, h, text, fID, bID, UserInfo);
	}
	int IOManager::CreateTimer(int ID,int x, int y, int w, int h, std::string text,int bID, int userInfo)
	{
		return menuManager->CreateTimer(ID, x, y, w, h, text, bID, userInfo);
	}
	int IOManager::CreateText(int ID,int x, int y, int w, int h, std::string text,int bID, int userInfo)
	{
		return menuManager->CreateText(ID, x, y, w, h, text, bID, userInfo);
	}
	void ClearLevel() {
		IOManager::getInstance()->getMenuManager()->Flush();
	}
};