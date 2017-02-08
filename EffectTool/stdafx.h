
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif




#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <timeapi.h>
#include <ctime>
#include <time.h>
#include <timeapi.h>
#include <queue>
#include <unordered_map>

// D3D11
//#include <d3dx11tex.h>
#include <D3DX11tex.h>

//
//// DirectX Math
//#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
//#include <DirectXColors.h>
//#include <DirectXCollision.h>
//#include <d3dx11.h>
//#include <d3d11.h>
//#include <D3DX11tex.h>

//#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
//#include <D3DX10Math.h>	//Direct3D 수학 함수를 사용하기 위한 헤더 파일
//#include <D3D9Types.h>
//#include <Mmsystem.h>
//#include <math.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// C++11
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <stack>
#include <memory>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;


//CB
//vs
#define VS_CB_CAMERA			0
#define VS_CB_MODEL				1
#define VS_CB_TEXTURE_MATRIX	2
#define VS_GLOBAL_BUFFER_SLOT	10
//ds
#define DS_CB_CAMERA			0		
//gs
#define GS_CB_CAMERA			0
#define GS_CB_AABB				1
//ps
#define PS_CB_MATERIAL 3
//CB

//TEXTURE
//ds
#define DS_SLOT_HEIGHTMAP		0
//ps
#define PS_TEXTURE 0x00
#define PS_SLOT_TERRAIN_BASE	0
#define PS_SLOT_TERRAIN_DETAIL	1
#define PS_SLOT_NORMALMAP		5
#define PS_SLOT_SKYBOX 13
//TEXTURE

//SAMPLER
#define PS_TEXTURE_SAMPLER		0
#define PS_NORMALMAP_SAMPLER	5
//SAMPLER

//DEFERRED_LIGHT
#define PS_UNPACKING_SLOT		0x00
#define PS_CAMERA_DATA_SLOT		0x01
#define PS_OBJECT_BUFFER_SLOT	0x02

#define DS_OBJECT_BUFFER_SLOT	0x00

#define MAX(a,b)(a>b? a : b)
#define MIN(a,b)(a<b? a : b)

//texture, constant buffer bind flag
#define BIND_VS 0b000001   // 0x0001 //0001
#define BIND_HS 0b000010   // 0x0002 //0010
#define BIND_DS 0b000100   // 0x0004 //0100
#define BIND_GS 0b001000   // 0x0008 //1000
#define BIND_PS 0b010000   // 0x0010 //...
#define BIND_CS 0b100000   // 0x0020 //..

//for loop를 위한 enum = OBJECT_END
enum object_id {
	OBJECT_SKYBOX,
	OBJECT_SPACE,
	OBJECT_TERRAIN,
	OBJECT_TEST,
	OBJECT_PLANE,
	OBJECT_FBX_ELF,
	OBJECT_FBX_BUNNY,
	OBJECT_END,

	OBJECT_DIRECTIONAL_LIGHT,
	OBJECT_POINT_LIGHT,
	OBJECT_SPOT_LIGHT,
	OBJECT_CAPSULE_LIGHT,
	OBJECT_LIGHT_END,

	OBJECT_POSTPROCESSING,
	OBJECT_POSTPROCESSING_END,

	OBJECT_DEBUG_AABB,
	OBJECT_DEBUG_COORD,
	OBJECT_DEBUG_POINT_LIGHT,
	OBJECT_DEBUG_CAPSULE_LIGHT,
	OBJECT_DEBUG_SPOT_LIGHT,
	OBJECT_DEBUG_TEXTURE,
	OBJECT_DEBUG_END
};

enum tag {
	TAG_DEFAULT,
	TAG_STATIC_OBJECT,
	TAG_DYNAMIC_OBJECT,
	TAG_END
};

enum shader_value_num {
	SVN_DEFAULT,
	SVN_DEBUG,
	SVN_END
};


//space
//space main info
#define SPACE_SIZE 1024.0f
#define SPACE_LEVEL 2.0f
//space main info

//한 사이드에 있는 공간의 개수
#define ONESIDE_SPACE_NUM  pow(2.0f, SPACE_LEVEL)
//공간 하나의 크기
#define ONESPACE_SIZE SPACE_SIZE / ONESIDE_SPACE_NUM
//공간의 수
#define SPACE_NUM  pow(ONESIDE_SPACE_NUM, 2.0f)
//space

//aabb
#define BOUNDINGBOX_NUM 5000
//coordinatesystem
#define COORD_NUM 5000
//singleton

#include "GlobalValueManager.h"
#define			GLOBALVALUEMGR	CGlobalValueManager::GetInstance()
#include "ResourceManager.h"
#define			RESOURCEMGR		CResourceManager::GetInstance()
#include "RenderContainerSeller.h"
#define			RCSELLER		CRenderContainerSeller::GetInstance()
#include "Renderer.h"
#define			RENDERER	CRenderer::GetInstance()
#define USE_ANIM
