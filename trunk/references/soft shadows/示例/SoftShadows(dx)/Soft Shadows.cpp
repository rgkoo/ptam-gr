/*-----------------------------------------------------------------------------
	Name	: Soft Shadows.cpp
	Desc	: Main source file.
	Author	: Anirudh S Shastry. Copyright (c) 2004.
	Date	: 22nd June, 2004.
-----------------------------------------------------------------------------*/

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include "resource.h"

#define SHADOW_MAP_SIZE	512
#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768

#define SAFE_RELEASE( x )	{ if( x ) { x->Release(); x = NULL; } }

//
// Gaussian functions
//
float GetGaussianDistribution( float x, float y, float rho ) {
    float g = 1.0f / sqrt( 2.0f * 3.141592654f * rho * rho );
    return g * exp( -(x * x + y * y) / (2 * rho * rho) );
}

void GetGaussianOffsets( bool bHorizontal, D3DXVECTOR2 vViewportTexelSize,
						 D3DXVECTOR2* vSampleOffsets, float* fSampleWeights ) {
    // Get the center texel offset and weight
    fSampleWeights[0] = 1.0f * GetGaussianDistribution( 0, 0, 2.0f );
    vSampleOffsets[0] = D3DXVECTOR2( 0.0f, 0.0f );
    
	// Get the offsets and weights for the remaining taps
	if( bHorizontal ) {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2(  i * vViewportTexelSize.x, 0.0f );
			vSampleOffsets[i + 1] = D3DXVECTOR2( -i * vViewportTexelSize.x, 0.0f );

			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( float(i + 0), 0.0f, 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( float(i + 1), 0.0f, 3.0f );
		}
	}

	else {
		for( int i = 1; i < 15; i += 2 ) {
			vSampleOffsets[i + 0] = D3DXVECTOR2( 0.0f,  i * vViewportTexelSize.y );
			vSampleOffsets[i + 1] = D3DXVECTOR2( 0.0f, -i * vViewportTexelSize.y );
			
			fSampleWeights[i + 0] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 0), 3.0f );
			fSampleWeights[i + 1] = 2.0f * GetGaussianDistribution( 0.0f, float(i + 1), 3.0f );
		}
	}
}

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
HWND				g_hWnd				= NULL;
LPDIRECT3D9			g_pD3D				= NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice		= NULL;

LPD3DXFONT			g_pFont				= NULL;

LPD3DXMESH			g_pScene			= NULL;
LPD3DXEFFECT		g_pEffect			= NULL;

LPDIRECT3DVERTEXBUFFER9	g_pQuadVB		= NULL;

LPDIRECT3DTEXTURE9	g_pColorMap_Floor	= NULL;
LPDIRECT3DTEXTURE9	g_pColorMap_Statue	= NULL;
LPDIRECT3DTEXTURE9	g_pSpotMap			= NULL;

LPDIRECT3DTEXTURE9	g_pShadowMap		= NULL;
LPDIRECT3DSURFACE9	g_pShadowSurf		= NULL;
LPDIRECT3DSURFACE9	g_pShadowDepth		= NULL;

LPDIRECT3DTEXTURE9	g_pScreenMap		= NULL;
LPDIRECT3DSURFACE9	g_pScreenSurf		= NULL;

LPDIRECT3DTEXTURE9	g_pBlurMap[2]		= {NULL};
LPDIRECT3DSURFACE9	g_pBlurSurf[2]		= {NULL};

LPDIRECT3DSURFACE9	g_pNewDepthRT		= NULL;
LPDIRECT3DSURFACE9	g_pOldColorRT		= NULL;
LPDIRECT3DSURFACE9	g_pOldDepthRT		= NULL;

D3DXVECTOR2			g_vSampleOffsets[15];
FLOAT				g_fSampleWeights[15];

LPDIRECT3DSURFACE9	g_pScreenshot		= NULL;
UINT				g_uNumScreenshots	= 0;

UINT				g_uFrameCount		= 0;
FLOAT				g_fStartTime		= 0.0f;
FLOAT				g_fFramerate		= 0.0f;
FLOAT				g_fStopTime			= 0.0f;
BOOL				g_bDisplayStats		= TRUE;

D3DXVECTOR3			g_vEyePos			= D3DXVECTOR3( -20.0f, 20.0f, -20.0f );
D3DXVECTOR3			g_vEyeAim			= D3DXVECTOR3(  20.0f,  0.0f,  20.0f );
D3DXVECTOR3			g_vUp				= D3DXVECTOR3(   0.0f,  1.0f,   0.0f );

BOOL				g_bWindowed			= FALSE;
BOOL				g_bWireframe		= FALSE;
BOOL				g_bSoftShadows		= TRUE;
BOOL				g_bFiltered			= TRUE;

// Vertex element
D3DVERTEXELEMENT9 dwElement[] =
{
	{ 0,   0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	
	{ 0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	
	{ 0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	
	D3DDECL_END()
};

struct QuadVertex
{
	D3DXVECTOR4 p;
	D3DXVECTOR2 t;
};

#define FVF_QUADVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Initialize();
HRESULT ShutDown();
HRESULT Render();
HRESULT FrameMove();
HRESULT ScreenGrab();

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow )
{
	WNDCLASSEX winClass;
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = (LPCSTR)IDR_MENU1;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( RegisterClassEx( &winClass) == 0 )
		return E_FAIL;

	if( g_bWindowed )
		g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", "Soft Shadows", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL );
	else
		g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", "Soft Shadows", WS_POPUP | WS_SYSMENU | WS_VISIBLE,
								 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	if( FAILED( Initialize() ) )
	{
		MessageBox( g_hWnd, "Unable to initialize Direct3D!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Get the starting time
	g_fStartTime = timeGetTime() * 0.001f;

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		{
		    FrameMove();
			Render();
			g_uFrameCount++;
			g_fFramerate = (FLOAT)g_uFrameCount / (timeGetTime() * 0.001f - g_fStartTime);
		}
	}
	
	// Get the stopping time
	g_fStopTime = timeGetTime() * 0.001f;

	if( FAILED( ShutDown() ) )
	{
		MessageBox( g_hWnd, "Unable to shutdown Direct3D!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}		

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_UP:
				{
					D3DXVECTOR3 vView;
					D3DXVec3Normalize( &vView, &(g_vEyeAim - g_vEyePos) );
					g_vEyePos.x += vView.x;
					g_vEyePos.z += vView.z;
					g_vEyeAim.x += vView.x;
					g_vEyeAim.z += vView.z;
					break;
				}
				
				case VK_DOWN:
				{	
					D3DXVECTOR3 vView;
					D3DXVec3Normalize( &vView, &(g_vEyeAim - g_vEyePos) );
					g_vEyePos.x -= vView.x;
					g_vEyePos.z -= vView.z;
					g_vEyeAim.x -= vView.x;
					g_vEyeAim.z -= vView.z;
					break;
				}

				case VK_LEFT:
				{	
					D3DXVECTOR3 vView, vStrafe;
					D3DXVec3Normalize( &vView, &(g_vEyeAim - g_vEyePos) );
					D3DXVec3Cross( &vStrafe, &vView, &g_vUp );
					g_vEyePos.x += vStrafe.x;
					g_vEyePos.z += vStrafe.z;
					g_vEyeAim.x += vStrafe.x;
					g_vEyeAim.z += vStrafe.z;
					break;
				}
				
				case VK_RIGHT:
				{	
					D3DXVECTOR3 vView, vStrafe;
					D3DXVec3Normalize( &vView, &(g_vEyeAim - g_vEyePos) );
					D3DXVec3Cross( &vStrafe, &vView, &g_vUp );
					g_vEyePos.x -= vStrafe.x;
					g_vEyePos.z -= vStrafe.z;
					g_vEyeAim.x -= vStrafe.x;
					g_vEyeAim.z -= vStrafe.z;
					break;
				}

				case 'Z':
				{
					g_vEyePos.y += 1.0f;
					break;
				}
				
				case 'X':
				{
					g_vEyePos.y -= 1.0f;
					break;
				}

				case 'W':
				{
					g_bWireframe = !g_bWireframe;
					break;
				}

				case 'S':
				{
					g_bSoftShadows = !g_bSoftShadows;
					break;
				}
				
				case 'F':
				{
					g_bFiltered = !g_bFiltered;
					break;
				}

				case 'G':
				{
					ScreenGrab();
					break;
				}

				case VK_F1:
				{
					g_bDisplayStats = !g_bDisplayStats;
				}

			}
		}
        break;

		case WM_CLOSE:
		{
			MessageBox( g_hWnd, "By Anirudh S Shastry. Copyright (c) 2004.", "About", MB_OK );
			PostQuitMessage(0);
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;
        
		case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
            {
				case ID_FILE_EXIT:
				{
                    SendMessage( g_hWnd, WM_CLOSE, 0, 0 );
                    return 0;
				}
 				
				case ID_HELP_ABOUT:
				{
					MessageBox( g_hWnd, "By Anirudh S Shastry. Copyright (c) 2004.", "About", MB_OK );
                    return 0;
				}

				case ID_FILE_TOGGLESTATS:
				{
					g_bDisplayStats = !g_bDisplayStats;
					return 0;
				}
 				
				default:
				{
					break;
				}
			}
			break;
		}

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: Initialize()
// Desc: Initialize Direct3D
//-----------------------------------------------------------------------------
HRESULT Initialize()
{
    // Create the Direct3D object
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( g_pD3D == NULL )
	{
		MessageBox( g_hWnd, "Unable to create the Direct3D object!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

    // Display mode
	D3DDISPLAYMODE d3ddm;

    // Get the suitable display mode
	UINT uNumModes = g_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8 );
    
	// Select the best mode (offering the highest frequency at the specified resolution)
	D3DDISPLAYMODE* pModes = new D3DDISPLAYMODE[uNumModes];
	for( UINT uMode = 0; uMode < uNumModes; uMode++ )
	{
		// Enumerate the adapter modes
		if( FAILED( g_pD3D->EnumAdapterModes( D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, uMode, &pModes[uMode] ) ) )
		{
			MessageBox( g_hWnd, "Unable to enumerate adapter display mode!", ERROR, MB_OK | MB_ICONERROR );
			return E_FAIL;
		}	
	}
	
	for( uMode = 0; uMode < uNumModes; uMode++ )
	{
		if( pModes[uMode].Width == SCREEN_WIDTH && pModes[uMode].Height == SCREEN_HEIGHT )
		{
			if( pModes[uMode + 1].Width == SCREEN_WIDTH && pModes[uMode + 1].Height == SCREEN_HEIGHT )
			{
				if( pModes[uMode].RefreshRate > pModes[uMode + 1].RefreshRate )
				{
					d3ddm = pModes[uMode];
				}
				else
				{
					d3ddm = pModes[uMode + 1];
				}
			}
			else
			{
				d3ddm = pModes[uMode];
			}
		}

		else if( pModes[uMode].Width > SCREEN_WIDTH || pModes[uMode].Height > SCREEN_HEIGHT ) {
			break;
		}
	}

	// Delete the list
	delete [] pModes;

	if( FAILED( g_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
										   d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
										   D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
	{
		MessageBox( g_hWnd, "Unable to find suitable display mode!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// If we are using windowed mode
	if( g_bWindowed )
		g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	if( FAILED( g_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
										   d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
										   D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
	{
		MessageBox( g_hWnd, "Unable to get adapter display mode!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Get the hardware caps
	D3DCAPS9 d3dCaps;

	if( FAILED( g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, 
		                               D3DDEVTYPE_HAL, &d3dCaps ) ) )
	{
		MessageBox( g_hWnd, "Unable to get hardware caps!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Check for vertex/pixel shaders 2.0 support
	if( d3dCaps.VertexShaderVersion < D3DVS_VERSION( 2, 0 ) || d3dCaps.PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
	{
		MessageBox( g_hWnd, "Vertex/Pixel shaders 2.0 not supported!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Check for R32F surface format support
	if( FAILED( g_pD3D->CheckDepthStencilMatch( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format,
												D3DFMT_R32F, D3DFMT_D16 ) ) )
	{
		MessageBox( g_hWnd, "R32F format not supported!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	DWORD dwBehaviorFlags = 0;

	if( d3dCaps.VertexProcessingCaps != 0 )
		dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.BackBufferWidth			= d3ddm.Width;
	d3dpp.BackBufferHeight			= d3ddm.Height;
	d3dpp.BackBufferFormat			= d3ddm.Format;
	d3dpp.BackBufferCount			= 1;
	
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow				= g_hWnd;
	d3dpp.Windowed					= g_bWindowed;

	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	if( g_bWindowed )
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	else
	{
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;
	}

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                                      dwBehaviorFlags, &d3dpp, &g_pd3dDevice ) ) )
	{
		MessageBox( g_hWnd, "Unable to create the Direct3D device!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Create the font object
    if( FAILED( D3DXCreateFont( g_pd3dDevice, 16, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, 
								OUT_TT_ONLY_PRECIS, 0, 0, "Veranda", &g_pFont ) ) )
	{
		MessageBox( g_hWnd, "Unable to create the font object!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Load the scene
	LPD3DXMESH pTempScene = NULL;
	if( FAILED( D3DXLoadMeshFromX( "Media/Soft Shadows.x", D3DXMESH_32BIT, g_pd3dDevice,
									NULL, NULL, NULL, NULL, &pTempScene ) ) )
	{
		MessageBox( g_hWnd, "Unable to load Soft Shadows.x", "Error", MB_OK | MB_ICONERROR );
		return E_FAIL;
	}
	pTempScene->CloneMesh( D3DXMESH_32BIT, dwElement, g_pd3dDevice, &g_pScene );

	SAFE_RELEASE( pTempScene );

	// Compute the normals and tangents for the scene
	D3DXComputeNormals( g_pScene, NULL );

	// Create the effect
	LPD3DXBUFFER pBufferErrors = NULL;
	if( FAILED( D3DXCreateEffectFromFile( g_pd3dDevice, "Media/Soft Shadows.fx", NULL, NULL, 0, 
										  NULL, &g_pEffect, &pBufferErrors ) ) )
	{
		LPVOID pErrors = pBufferErrors->GetBufferPointer();
		MessageBox(NULL, (const char*)pErrors, ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	SAFE_RELEASE( pBufferErrors );
	
	// Create the quad vertex buffer
	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY, FVF_QUADVERTEX,
												  D3DPOOL_DEFAULT, &g_pQuadVB, NULL ) ) )
	{
		MessageBox( g_hWnd, "Unable to create vertex buffer!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Copy the vertices
	QuadVertex* pVertices;
	g_pQuadVB->Lock( 0, 0, (void**)&pVertices, 0 );
	pVertices[0].p = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f );
	pVertices[1].p = D3DXVECTOR4( 0.0f, SCREEN_HEIGHT / 2, 0.0f, 1.0f );
	pVertices[2].p = D3DXVECTOR4( SCREEN_WIDTH / 2, 0.0f, 0.0f, 1.0f );
	pVertices[3].p = D3DXVECTOR4( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, 1.0f );
	
	pVertices[0].t = D3DXVECTOR2( 0.0f, 0.0f );
	pVertices[1].t = D3DXVECTOR2( 0.0f, 1.0f );
	pVertices[2].t = D3DXVECTOR2( 1.0f, 0.0f );
	pVertices[3].t = D3DXVECTOR2( 1.0f, 1.0f );
	g_pQuadVB->Unlock();

	// Load the color and spot maps
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, "Media/Color_Floor.dds", &g_pColorMap_Floor ) ) )
	{
		MessageBox( g_hWnd, "Unable to load Color_Floor.dds", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}
	
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, "Media/Color_Statue.dds", &g_pColorMap_Statue ) ) )
	{
		MessageBox( g_hWnd, "Unable to load Color_Statue.dds", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}
		
	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, "Media/SpotMap.dds", &g_pSpotMap ) ) )
	{
		MessageBox( g_hWnd, "Unable to load SpotMap.dds", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Create the shadow map
	if( FAILED( g_pd3dDevice->CreateTexture( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1, D3DUSAGE_RENDERTARGET,
											 D3DFMT_R32F, D3DPOOL_DEFAULT, &g_pShadowMap, NULL ) ) )
	{
		MessageBox( g_hWnd, "Unable to create shadow map!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Grab the texture's surface
	g_pShadowMap->GetSurfaceLevel( 0, &g_pShadowSurf );

	// Create the screen-sized buffer map
	if( FAILED( g_pd3dDevice->CreateTexture( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, D3DUSAGE_RENDERTARGET,
											 D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pScreenMap, NULL ) ) )
	{
		MessageBox( g_hWnd, "Unable to create screen map!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Grab the texture's surface
	g_pScreenMap->GetSurfaceLevel( 0, & g_pScreenSurf );

	// Create the blur maps
	for( int i = 0; i < 2; i++ )
	{
		if( FAILED( g_pd3dDevice->CreateTexture( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, D3DUSAGE_RENDERTARGET,
												 D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pBlurMap[i], NULL ) ) )
		{
			MessageBox( g_hWnd, "Unable to create blur map!", ERROR, MB_OK | MB_ICONERROR );
			return E_FAIL;
		}

		// Grab the texture's surface
		g_pBlurMap[i]->GetSurfaceLevel( 0, & g_pBlurSurf[i] );
	}
	
	// Create the shadow depth surface
	if( FAILED( g_pd3dDevice->CreateDepthStencilSurface( SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, D3DFMT_D16,
														 D3DMULTISAMPLE_NONE, 0, TRUE, &g_pShadowDepth, NULL ) ) )
	{
		MessageBox( g_hWnd, "Unable to create shadow depth surface!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	// Create the general depth surface
	if( FAILED( g_pd3dDevice->CreateDepthStencilSurface( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, D3DFMT_D16,
														 D3DMULTISAMPLE_NONE, 0, TRUE, &g_pNewDepthRT, NULL ) ) )
	{
		MessageBox( g_hWnd, "Unable to create general depth surface!", ERROR, MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ShutDown()
// Desc: ShutDown Direct3D
//-----------------------------------------------------------------------------
HRESULT ShutDown()
{
	// Clean up
	SAFE_RELEASE( g_pFont );

	SAFE_RELEASE( g_pScene );
	SAFE_RELEASE( g_pEffect );

	SAFE_RELEASE( g_pColorMap_Floor );
	SAFE_RELEASE( g_pColorMap_Statue );
	SAFE_RELEASE( g_pSpotMap );
	
	SAFE_RELEASE( g_pShadowMap );
	SAFE_RELEASE( g_pShadowSurf );
	SAFE_RELEASE( g_pShadowDepth );

	SAFE_RELEASE( g_pScreenMap );
	SAFE_RELEASE( g_pScreenSurf );

	SAFE_RELEASE( g_pBlurMap[0] );
	SAFE_RELEASE( g_pBlurSurf[0] );
	SAFE_RELEASE( g_pBlurMap[1] );
	SAFE_RELEASE( g_pBlurSurf[1] );

	SAFE_RELEASE( g_pNewDepthRT );
	SAFE_RELEASE( g_pOldColorRT );
	SAFE_RELEASE( g_pOldDepthRT );

	SAFE_RELEASE( g_pd3dDevice );
	SAFE_RELEASE( g_pD3D );
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Setup effect variables and update stuff
//-----------------------------------------------------------------------------
HRESULT FrameMove()
{
	//
	// Camera space matrices
	//
	
	// Computee the world matrix
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	// Compute the view matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH( &matView, &g_vEyePos, &g_vEyeAim, &g_vUp );

	// Compute the projection matrix
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(60.0f), (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 1024.0f );

	// Compute the world-view-projection matrix
	D3DXMATRIX matWorldViewProj = matWorld * matView * matProj;

	// World inverse matrix
	D3DXMATRIX matWorldIT;
	D3DXMatrixInverse( &matWorldIT, NULL, &matWorld );
	D3DXMatrixTranspose( &matWorldIT, &matWorldIT );

	//
	// Light space matrices
	//
	
	// View matrix
	D3DXVECTOR3 vLightPos = D3DXVECTOR3(  40.0f, 60.0f, -40.0f );
	D3DXVECTOR3 vLightAim = D3DXVECTOR3(   0.0f,  0.0f,   0.0f );

	D3DXMatrixLookAtLH( &matView, &vLightPos, &vLightAim, &g_vUp );

	// Compute the projection matrix
	D3DXMatrixOrthoLH( &matProj, 45.0f, 45.0f, 1.0f, 1024.0f );

	// Compute the light-view-projection matrix
	D3DXMATRIX matLightViewProj = matWorld * matView * matProj;
	
	// Compute the texture matrix
	float fTexOffs = 0.5 + (0.5 / (float)SHADOW_MAP_SIZE);
	D3DXMATRIX matTexAdj( 0.5f,		0.0f,	0.0f,	0.0f,
						  0.0f,    -0.5f,	0.0f,	0.0f,
						  0.0f,		0.0f,	1.0f,	0.0f,
						  fTexOffs, fTexOffs,  0.0f, 1.0f );

	D3DXMATRIX matTexture = matLightViewProj * matTexAdj;

	// Setup the effect variables
	g_pEffect->SetMatrix( "g_matWorldViewProj", &matWorldViewProj );
	g_pEffect->SetMatrix( "g_matLightViewProj", &matLightViewProj );
	g_pEffect->SetMatrix( "g_matWorld", &matWorld );
	g_pEffect->SetMatrix( "g_matWorldIT", &matWorldIT );
	g_pEffect->SetMatrix( "g_matTexture", &matTexture );

	g_pEffect->SetVector( "g_vLightPos", (D3DXVECTOR4*)&vLightPos );
	g_pEffect->SetVector( "g_vEyePos", (D3DXVECTOR4*)&g_vEyePos );
	g_pEffect->SetVector( "g_vLightColor", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 0.5f ) );

	g_pEffect->SetBool("g_bFiltered", g_bFiltered);
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Render the frame
//-----------------------------------------------------------------------------
HRESULT Render()
{
    // Clear the viewport
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

	// Begin rendering the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		// Grab the old render target and depth buffer
		g_pd3dDevice->GetRenderTarget( 0, &g_pOldColorRT );
		g_pd3dDevice->GetDepthStencilSurface( &g_pOldDepthRT );
    
		// Render the scene depth to the shadow map
		g_pd3dDevice->SetRenderTarget( 0, g_pShadowSurf );
		g_pd3dDevice->SetDepthStencilSurface( g_pShadowDepth );
    
		// Clear the viewport
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );

		// Set the technique
		g_pEffect->SetTechnique( "techShadow" );

		// Render the effect
		UINT uPasses = 0;
		g_pEffect->Begin( &uPasses, 0 );

		for( UINT uPass = 0; uPass < uPasses; uPass++ )
		{
			// Set the current pass
			g_pEffect->BeginPass( uPass );

			// Draw the floor
			g_pScene->DrawSubset(0);
			
			// Draw the statue
			g_pScene->DrawSubset(1);
			g_pScene->DrawSubset(2);

			// End the current pass
			g_pEffect->EndPass();
		}

		// End the effect
		g_pEffect->End();
		
		if( g_bSoftShadows )
		{
			// Render the shadowed scene into the screen map
			g_pd3dDevice->SetRenderTarget( 0, g_pScreenSurf );
			g_pd3dDevice->SetDepthStencilSurface( g_pNewDepthRT );
    
			// Clear the viewport
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

			// Set the technique
			g_pEffect->SetTechnique( "techUnlit" );
			
			// Set the textures
			g_pEffect->SetTexture( "tShadowMap", g_pShadowMap );

			// Render the effect
			uPasses = 0;
			g_pEffect->Begin( &uPasses, 0 );

			for( uPass = 0; uPass < uPasses; uPass++ )
			{
				// Set the current pass
				g_pEffect->BeginPass( uPass );

				// Draw the floor
				g_pScene->DrawSubset(0);
				
				// Draw the statue
				g_pScene->DrawSubset(1);
				g_pScene->DrawSubset(2);

				// End the current pass
				g_pEffect->EndPass();
			}

			// End the effect
			g_pEffect->End();

			// Blur the screen map horizontally
			g_pd3dDevice->SetRenderTarget( 0, g_pBlurSurf[0] );
			g_pd3dDevice->SetDepthStencilSurface( g_pNewDepthRT );
    
			// Clear the viewport
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

			// Set the technique
			g_pEffect->SetTechnique( "techBlurH" );
			
			// Compute the Gaussian offsets
			GetGaussianOffsets(TRUE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
							   g_vSampleOffsets, g_fSampleWeights);
			g_pEffect->SetValue("g_vSampleOffsets", g_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
			g_pEffect->SetValue("g_fSampleWeights", g_fSampleWeights, 15 * sizeof(FLOAT));

			// Set the textures
			g_pEffect->SetTexture( "tScreenMap", g_pScreenMap );

			// Render the effect
			uPasses = 0;
			g_pEffect->Begin( &uPasses, 0 );

			for( uPass = 0; uPass < uPasses; uPass++ )
			{
				// Set the current pass
				g_pEffect->BeginPass( uPass );

				// Draw the quad
				g_pd3dDevice->SetStreamSource( 0, g_pQuadVB, 0, sizeof(QuadVertex) );
				g_pd3dDevice->SetFVF( FVF_QUADVERTEX );
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
				
				// End the current pass
				g_pEffect->EndPass();
			}

			// End the effect
			g_pEffect->End();
			
			// Blur the screen map vertically
			g_pd3dDevice->SetRenderTarget( 0, g_pBlurSurf[1] );
			g_pd3dDevice->SetDepthStencilSurface( g_pNewDepthRT );
    
			// Clear the viewport
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

			// Set the technique
			g_pEffect->SetTechnique( "techBlurV" );
			
			// Compute the Gaussian offsets
			GetGaussianOffsets(FALSE, D3DXVECTOR2(1.0f / (FLOAT)SHADOW_MAP_SIZE, 1.0f / (FLOAT)SHADOW_MAP_SIZE),
							   g_vSampleOffsets, g_fSampleWeights);
			g_pEffect->SetValue("g_vSampleOffsets", g_vSampleOffsets, 15 * sizeof(D3DXVECTOR2));
			g_pEffect->SetValue("g_fSampleWeights", g_fSampleWeights, 15 * sizeof(FLOAT));

			// Set the textures
			g_pEffect->SetTexture( "tBlurHMap", g_pBlurMap[0] );

			// Render the effect
			uPasses = 0;
			g_pEffect->Begin( &uPasses, 0 );

			for( uPass = 0; uPass < uPasses; uPass++ )
			{
				// Set the current pass
				g_pEffect->BeginPass( uPass );

				// Draw the quad
				g_pd3dDevice->SetStreamSource( 0, g_pQuadVB, 0, sizeof(QuadVertex) );
				g_pd3dDevice->SetFVF( FVF_QUADVERTEX );
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

				// End the current pass
				g_pEffect->EndPass();
			}

			// End the effect
			g_pEffect->End();

			// Restore the render target and depth buffer
			g_pd3dDevice->SetRenderTarget( 0, g_pOldColorRT );
			g_pd3dDevice->SetDepthStencilSurface( g_pOldDepthRT );
			SAFE_RELEASE( g_pOldColorRT );
			SAFE_RELEASE( g_pOldDepthRT );

			// Finally, render the shadowed scene
			g_pEffect->SetTechnique( "techScene" );
			
			// Set the textures
			g_pEffect->SetTexture( "tBlurVMap", g_pBlurMap[1] );
			g_pEffect->SetTexture( "tSpotMap", g_pSpotMap );

			// Render the effect
			uPasses = 0;
			g_pEffect->Begin( &uPasses, 0 );

			// Do we need to render the scene in wireframe mode
			if( g_bWireframe )
				g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

			for( uPass = 0; uPass < uPasses; uPass++ )
			{
				// Set the current pass
				g_pEffect->BeginPass( uPass );

				// Draw the floor
				g_pEffect->SetTexture( "tColorMap", g_pColorMap_Floor );
				g_pEffect->CommitChanges();
				g_pScene->DrawSubset(0);
				
				// Draw the statue
				g_pEffect->SetTexture( "tColorMap", g_pColorMap_Statue );
				g_pEffect->CommitChanges();
				g_pScene->DrawSubset(1);
				g_pScene->DrawSubset(2);

				// End the current pass
				g_pEffect->EndPass();
			}

			// End the effect
			g_pEffect->End();
			
			// Restore the render state
			g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		}

		else
		{
			// Restore the render target and depth buffer
			g_pd3dDevice->SetRenderTarget( 0, g_pOldColorRT );
			g_pd3dDevice->SetDepthStencilSurface( g_pOldDepthRT );
			SAFE_RELEASE( g_pOldColorRT );
			SAFE_RELEASE( g_pOldDepthRT );

			// Otherwise, render the scene with hard shadows
			g_pEffect->SetTechnique( "techSceneHard" );
			
			// Set the textures
			g_pEffect->SetTexture( "tShadowMap", g_pShadowMap );
			g_pEffect->SetTexture( "tSpotMap", g_pSpotMap );

			// Render the effect
			uPasses = 0;
			g_pEffect->Begin( &uPasses, 0 );

			// Do we need to render the scene in wireframe mode
			if( g_bWireframe )
				g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

			for( uPass = 0; uPass < uPasses; uPass++ )
			{
				// Set the current pass
				g_pEffect->BeginPass( uPass );

				// Draw the floor
				g_pEffect->SetTexture( "tColorMap", g_pColorMap_Floor );
				g_pEffect->CommitChanges();
				g_pScene->DrawSubset(0);
				
				// Draw the statue
				g_pEffect->SetTexture( "tColorMap", g_pColorMap_Statue );
				g_pEffect->CommitChanges();
				g_pScene->DrawSubset(1);
				g_pScene->DrawSubset(2);

				// End the current pass
				g_pEffect->EndPass();
			}

			// End the effect
			g_pEffect->End();
			
			// Restore the render state
			g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		}

		// Display the frame stats
		if( g_bDisplayStats )
		{
			TCHAR szStats[255];
			sprintf( szStats, "Framerate : %2.1f", g_fFramerate );
			
			RECT rc = { 5, 5, 0, 0 };
			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_CALCRECT, 0xFFFFAA00 );
 			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_NOCLIP,   0xFFFFAA00 );

			sprintf( szStats, "Use arrow and Z, X keys to move around the scene" );
			SetRect( &rc, 5, SCREEN_HEIGHT - 40, 0, 0 );
			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_CALCRECT, 0xFFFFAA00 );
 			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_NOCLIP,   0xFFFFAA00 );
			
			sprintf( szStats, "Press W to toggle wireframe display" );
			SetRect( &rc, 5, SCREEN_HEIGHT - 20, 0, 0 );
			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_CALCRECT, 0xFFFFAA00 );
 			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_NOCLIP,   0xFFFFAA00 );
			
			sprintf( szStats, "Press S to toggle soft shadows" );
			SetRect( &rc, 350, SCREEN_HEIGHT - 40, 0, 0 );
			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_CALCRECT, 0xFFFFAA00 );
 			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_NOCLIP,   0xFFFFAA00 );
				
			sprintf( szStats, "Press G to pump out a screenshot" );
			SetRect( &rc, 350, SCREEN_HEIGHT - 20, 0, 0 );
			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_CALCRECT, 0xFFFFAA00 );
 			g_pFont->DrawText( NULL, szStats, -1, &rc, DT_NOCLIP,   0xFFFFAA00 );
		}

		// End rendering the scene and present it
		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ScreenGrab()
// Desc: Saves the backbuffer to a file
//-----------------------------------------------------------------------------
HRESULT ScreenGrab()
{
	//
	// Grab the back buffer and save it to a file
	//
	
	D3DDISPLAYMODE d3ddm;
	g_pd3dDevice->GetDisplayMode( 0, &d3ddm );

	g_pd3dDevice->CreateOffscreenPlainSurface( d3ddm.Width, d3ddm.Height, D3DFMT_A8R8G8B8,
											   D3DPOOL_DEFAULT, &g_pScreenshot, NULL );

	g_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &g_pScreenshot );

	TCHAR szScreenshot[32];
	sprintf( szScreenshot, "ScreenShot%d.bmp", g_uNumScreenshots++ );
	D3DXSaveSurfaceToFile( szScreenshot, D3DXIFF_BMP, g_pScreenshot, NULL, NULL );

	SAFE_RELEASE( g_pScreenshot );

	return S_OK;
}