//GraphicsCore.cpp

//Init//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Create Sprite Object and Textures
//////////////////////////////////////////////////////////////////////////
/*
// Create a sprite object, note you will only need one for all 2D sprites
D3DXCreateSprite(d3dDevice, &m_pD3DSprite);
		
// Create a texture, each different 2D sprite to display to the screen
// will need a new texture object.  If drawing the same sprite texture
// multiple times, just call that sprite's Draw() with different 
// transformation values.
D3DXCreateTextureFromFileEx(d3dDevice, L"cube.png", 0, 0, 0, 0, 
	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
	D3DCOLOR_XRGB(255, 0, 255), &m_imageInfo, 0, &m_pTexture);
		
//////////////////////////////////////////////////////////////////////////
// Sprite Sheets
//////////////////////////////////////////////////////////////////////////
D3DXGetImageInfoFromFile(L"fairy_sheet.png", &imageInfo); //Get the width and height info from bitmap
m_pD3DDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
D3DXLoadSurfaceFromFile(surface, NULL, NULL, L"fairy_sheet.png", NULL, D3DX_DEFAULT, 0, NULL);
	
D3DXCreateTextureFromFile(m_pD3DDevice, L"fairy_sheet.png", &m_pTexture2);


D3DXGetImageInfoFromFile(L"explosion.png", &m_imageInfo3); //Get the width and height info from bitmap
m_pD3DDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
D3DXLoadSurfaceFromFile(surface, NULL, NULL, L"explosion.png", NULL, D3DX_DEFAULT, 0, NULL);
	
D3DXCreateTextureFromFile(m_pD3DDevice, L"explosion.png", &m_pTexture3);*/



//Shutdown///////////////////////////////////////////////


/*
// Release COM objects in the opposite order they were created in
SAFE_RELEASE(m_pDIKeyboard);
SAFE_RELEASE(m_pDIMouse);
SAFE_RELEASE(m_pDIObject);
*/


//////////////////////////////////////////////////////////
/*
	void GraphicsCore::DrawText(char *str, int x, int y, int w, int h)
	{
		if( !inRender )
		{
			return;
		}

		m_pD3DFont->DrawTextA(0, str, -1, &rect, DT_VCENTER | DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	void GraphicsCore::spriteDisplay(int id, int numFrames, int numRows, float spriteLocationX, float spriteLocationY)
	{
		DWORD timeNow = timeGetTime();

		RECT drawRect;

		//Sprite *sprt = spriteManager[id];

		if( !sprt ) return;

		//Start Width of Each Animation Frame
		drawRect.left = sprt->curFrame * 64;
		//End Width of Each Animation Frame
		drawRect.right = drawRect.left + 64;
		//Start Height of Frame
		drawRect.top = top;

	
			//Go through each frame until it reaches the amount of frames
		//there are in each row
		if(sprt->curFrame < numFrames)
		{
			sprt->curFrame++;
		}
		//When finished displaying last frame of the current row then
		//increment row number by 1 and move down 64 pixels to next row
		else
		{
			endOfRow += 1;
			top += 65;
			sprt->curFrame = 0;
		}
		//If it has reached the max number of rows in the image, then
		//reset the animation by reseting top pixel, current frame, 
		//row count back to 0 and as well as returning false to tell that
		//it has reached the end of the animation and to stop unless looping
		if(endOfRow > numRows)
		{
			top = 0;
			sprt->curFrame = 0;
			endOfRow = 0;
			return;
		}

		//Height of Each Frame from the top to the bottom
		drawRect.bottom = drawRect.top + 64;

		D3DXMATRIX transMat, rotMat, scaleMat, worldMat;
		D3DXMatrixIdentity(&transMat);
		D3DXMatrixIdentity(&scaleMat);
		D3DXMatrixIdentity(&rotMat);
		D3DXMatrixIdentity(&worldMat);
	
		D3DXMatrixScaling(&scaleMat, 2.0f, 2.0f, 0.0f);								// Scaling
		D3DXMatrixRotationZ(&rotMat, 0.0f);											//Rotation
		D3DXMatrixTranslation(&transMat, spriteLocationX, spriteLocationY, 0.0f);	// Translation
		D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);							// Multiply scale and rotation, store in scale
		D3DXMatrixMultiply(&worldMat, &scaleMat, &transMat);						// Multiply scale and translation, store in world

		// Set Transform
		m_pD3DSprite->SetTransform(&worldMat);

		m_pD3DSprite->Draw(m_pTexture3,&drawRect,NULL, &D3DXVECTOR3(m_imageInfo3.Width * 0.5f, m_imageInfo3.Height * 0.5f, 0.0f),0xFFFFFFFF);
	}*/