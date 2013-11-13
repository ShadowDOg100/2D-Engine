#include "DirectInterface.h"
#include "GraphicsCore.h"

namespace GraphicsCore
{
	extern D3DPRESENT_PARAMETERS D3Dpp;

	void DirectInterface::Intialize()
	{
		HWND hWnd = gethWnd();

		// Create the D3D Object
		m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

		//Check device capabilities
		DWORD deviceBehaviorFlags = 0;
		m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

		// Determine vertex processing mode
		if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		{
			// Hardware vertex processing supported? (Video Card)
			deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
		}
		else
		{
			// If not, use software (CPU)
			deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
		}
	
		// If hardware vertex processing is on, check pure device support
		if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		{
			deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
		}
	
		// Create the D3D Device with the present parameters and device flags above
		m_pD3DObject->CreateDevice(
			D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
			D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
			hWnd,					// handle to the focus window
			deviceBehaviorFlags,	// behavior flags
			&D3Dpp,					// presentation parameters
			&m_pD3DDevice);			// returned device pointer
		SetMouseScale();
	}

	DirectInterface::DirectInterface(){};
	DirectInterface::~DirectInterface(){};
	void DirectInterface::SetMouseScale()
	{
		RECT rect;
		GetClientRect(gethWnd(),&rect);

		mouseScaleX=(double)SCREEN_WIDTH/(double)(rect.right-rect.left);
		mouseScaleY=(double)SCREEN_HEIGHT/(double)(rect.bottom-rect.top);
	}

	void DirectInterface::Shutdown()
	{
		SAFE_RELEASE(m_pD3DObject);
		SAFE_RELEASE(m_pD3DDevice);
	}
};