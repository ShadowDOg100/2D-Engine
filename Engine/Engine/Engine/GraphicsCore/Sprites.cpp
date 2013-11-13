#include "GraphicsCore.h"
#include "DirectInterface.h"
#include "Sprites.h"

namespace GraphicsCore
{
	extern DirectInterface DI;

	void Sprites::Initialize()
	{
		//Create the D3D sprite interface
		D3DXCreateSprite(DI.GetD3DDevice(), &m_pD3DSprite);

		spriteID = 0;
	}
	void Sprites::ClearSprites()
	{
		spriteManager.Flush();
	}
	int TextureManager::AddTexture(LPCSTR name)
	{
		for(int i = 0; i<textures.Top(); i++)
		{
			if( textures[i] &&  _stricmp(name,textures[i]->fileName)==0 )
				return i;
		}
			
		spriteTexture *st = new spriteTexture;
		strcpy_s(st->fileName,256,name);
		D3DXCreateTextureFromFileEx(
			DI.GetD3DDevice(),
			name,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&(st->texture));		
		return textures.Add(st);
	}
	Sprite::Sprite(int texID,int width, int height, int left, int top, int frames, bool loop):
					frameTime(.100),render(false),Observer()
	{
		spriteWidth = width;
		spriteHeight = height;

		spriteLocation.left = left;
		spriteLocation.top = top;
		spriteLocation.right = spriteWidth;
		spriteLocation.bottom = spriteHeight;

		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		center = D3DXVECTOR3((FLOAT)spriteWidth/2.0f, (FLOAT)spriteHeight/2.0f, 0);

		loopAnimation = loop;

		numFrames = frames;
		curFrame = 0;
	
		textureID = texID;

		//render = true;
	}

	bool Sprite::Notify(int from, int to, int type, int qual, MSGData p1, MSGData p2)
	{
		if(from == ID() )
		{
			if( render ) // this message is from me
			{
				curFrame++;
				if( curFrame >= numFrames )
				{
					if( loopAnimation )
						curFrame = 0;
					else
					{
						curFrame = -1;
						render = false;
						// add message here for animation complete
						return true;
					}
				}
				MSGData p1,p2;
				RemindMe((float)frameTime,-1,-1,p1,p2);
			}
			return true;
		}
		return false;
	}

	int Sprites::CreateSprite(LPCSTR file, int width, int height, int left, int top, int frames, bool loop)
	{		
		int textureID = textures.AddTexture(file);

		return spriteManager.Add(new Sprite(textureID,width,height,left,top,frames,loop));
	}

	void Sprite::Draw(int width, int height, int x, int y,ID3DXSprite *pSprite,TextureManager *tm)
	{
		if( curFrame < 0 ) return;

		int camOffX=0, camOffY=0;
		GetCameraOffset(camOffX,camOffY);
		x-=camOffX;
		y-=camOffY;
	
		pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);

		D3DXMATRIX transMat, scaleMat, worldMat;
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&worldMat);

		//Set the animation frame.
		spriteLocation.left = spriteWidth * curFrame;
		spriteLocation.right = spriteWidth * curFrame + spriteWidth;

		//Set sprite scale
		if( width <= 0 || height <=0 )
		{
			scale = D3DXVECTOR3(1.0, 1.0, 0);
		}
		else
		{
			scale = D3DXVECTOR3(((float)width/spriteWidth), ((float)height/spriteHeight), 0);
		}

		//Set sprite position
		position = D3DXVECTOR3((float)x, (float)y, 0);

		//Center the Sprite
		//D3DXVECTOR3 center(position);
		center.x = 0;
		center.y = 0;


		D3DXMatrixScaling(&scaleMat, scale.x, scale.y, 0.0f);			// Scaling
		D3DXMatrixTranslation(&transMat, position.x, position.y, 0.0f);	// Translation
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);	// Multiply scale and translation, store in world

		// Set Transform
		pSprite->SetTransform(&worldMat);

		//Draw the sprite.
		spriteTexture *st = tm->GetTexture(textureID);
		if( st )
		{
			pSprite->Draw(st->texture, &spriteLocation, &center, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		//End the sprite draw call.
		pSprite->End();
		if( !render && numFrames>1 )
		{
			render = true;
			MSGData p1,p2;
			RemindMe((float)frameTime,-1,-1,p1,p2);
		}
	}

	void Sprites::Draw(int ID, int width, int height, int x, int y)
	{
		Sprite *sprt = spriteManager[ID];
		if( !sprt ) return;

		sprt->Draw(width,height,x,y,m_pD3DSprite,&textures);
	}

	Sprites::Sprites(){};
	Sprites::~Sprites(){};

	void Sprites::shutdown()
	{
		m_pD3DSprite->Release();
	}
}