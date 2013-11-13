#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "../Common/observer.h"
//#include "../Common/linklist.h"

#pragma once

namespace GraphicsCore
{
	class Sprites;
	class TextureManager;
	class GraphicsCore;

	struct spriteTexture
	{
			char fileName[256];
			LPDIRECT3DTEXTURE9 texture;
	};

	class TextureManager
	{
	private:
		GenArray<spriteTexture> textures;

	public:
		TextureManager(){}
		~TextureManager(){}

		spriteTexture *GetTexture(int id)
		{
			return textures[id];
		}

		int AddTexture(LPCSTR name);

	};

	class Sprite: public Observer
	{
	private:
		D3DXVECTOR3 position;			// Translation
		D3DXVECTOR3 scale;				// Uniform scaling
		D3DXVECTOR3 center;				//Center of Image
		float rotation;					// Z rotation
		int spriteWidth;
		int spriteHeight;

		RECT spriteLocation;			//hold location of the sprite in the bitmap
		
		int numFrames;					//number of frames the animation has
		int curFrame;					//the current frame of animation
		double frameTime;				// the time a particular frame will be displayed.
		int textureID;


		bool loopAnimation;
		bool render;

		int topPixel;	

		DWORD lastUpdate;
	public:
		Sprite(int texID,int width, int height, int l, int t, int frames, bool loop);
		virtual ~Sprite(){};

		void Draw(int width, int height, int x, int y,ID3DXSprite *pSprite, TextureManager * tm);
		virtual bool Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2);
	};

	class Sprites
	{
	private:

		ID3DXSprite*		m_pD3DSprite;	// Sprite Object

		int frames;

		GenArray<Sprite> spriteManager;
		TextureManager textures;

	public:
		Sprites();
		~Sprites();

		int spriteID;

		void Initialize();

		int CreateSprite(LPCSTR file, int width, int height, int left, int top, int frames, bool loop);

		void startDraw();

		void Draw(int ID, int width, int height, int x, int y);

		void finishDraw();

		void shutdown();
		void ClearSprites();
	};
}