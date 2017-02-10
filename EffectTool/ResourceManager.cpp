#include "stdafx.h"
#include "ResourceManager.h"

bool CResourceManager::Begin(){
	CreateTextures();
	CreateRenderShaders();
	CreateBuffers();
	//CreateGlobalBuffers();
	CreateMaterials();
	CreateMeshs();


	return true;
}

bool CResourceManager::End(){
	ReleaseTextures();
	ReleaseRenderShaders();
	ReleaseBuffers();
	//ReleaseGlobalBuffers();
	ReleaseMaterials();
	ReleaseMeshs();

	return true;
}

void CResourceManager::CreateTextures(){
	//
	//texture
	shared_ptr<CTexture> pTexture;
	shared_ptr<CSampler> pSampler;
	//skybox
	int nIndex = 0;
	_TCHAR pstrTextureNames[128];
	_stprintf_s(pstrTextureNames, _T("../../Assets/SkyBox_%d.dds"), nIndex);
	//ID3D11ShaderResourceView *pd3dSRV = NULL;
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, pstrTextureNames, NULL, NULL, &pd3dSRV, NULL);
	
	UINT Slot = { PS_SLOT_SKYBOX };
	UINT BindFlag = { BIND_PS };
	//make sampler
	pSampler = make_shared<CSampler>();
	pSampler->Begin();
	m_mSampler.insert(pairSampler("SkyBox", pSampler));
	pTexture = make_shared<CTexture>();
	pTexture->Begin(pstrTextureNames, pSampler, Slot, BindFlag);
	m_mTexture.insert(pairTexture("SkyBox", pTexture));

	pSampler = make_shared<CSampler>();
	pSampler->Begin(PS_TEXTURE_SAMPLER, BIND_PS);
	m_mSampler.insert(pairSampler("DEFAULT", pSampler));
	UINT DefaultSlot = { PS_TEXTURE };
	UINT DefaultFlag = { BIND_PS };
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/default.jpg"), NULL, NULL, &pd3dSRV, NULL);
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/default.jpg"), pSampler, DefaultSlot, DefaultFlag);
	m_mTexture.insert(pairTexture("DEFAULT", pTexture));
	////elf
	////0 Çã¸®¶ì
	//UINT ElfSlot = { PS_TEXTURE };
	//UINT ElfBindFlag = { BIND_PS };
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Belt_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_0", pTexture));
	////1 ½Å¹ß
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Boots_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_1", pTexture));
	////2 Àå°©
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Glove_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_2", pTexture));
	////3 ¸Ó¸®Àå½Ä
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Helmet_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_3", pTexture));
	////4 ÇÏÀÇ
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Lower_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_4", pTexture));
	////5 »óÀÇ
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Upper_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_5", pTexture));
	////6 ¾î±ú Àå½Ä?
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Helmet02_D_HEY.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_6", pTexture));
	////7 ¾ó±¼
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_Face01_001_SP_BJH.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_7", pTexture));
	////8 ±Í
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_Face01_001_SP_BJH.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_8", pTexture));
	////9 ¸Ó¸®Ä«¶ô
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_Refer_Hair104_D_KDE.jpg"), NULL, NULL, &pd3dSRV, NULL);
	//pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
	//pTexture->Begin(pd3dSRV, pSampler, ElfSlot, ElfBindFlag);
	//m_mTexture.insert(pairTexture("FBX_9", pTexture));
	////elf

	//terrain heightmap
	//make sampler
	pSampler = make_shared<CSampler>();
	pSampler->Begin(PS_TEXTURE_SAMPLER, BIND_DS, D3D11_TEXTURE_ADDRESS_CLAMP);
	m_mSampler.insert(pairSampler("TerrainHeightMap", pSampler));
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/HeightMap.jpg"), NULL, NULL, &pd3dSRV, NULL);
	UINT HeightMapSlot = { DS_SLOT_HEIGHTMAP };
	UINT HeightMapBindFlag = { BIND_DS };
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/HeightMap.jpg"), pSampler, HeightMapSlot, HeightMapBindFlag);
	m_mTexture.insert(pairTexture("TerrainHeightMap", pTexture));
	//terrain base texture
	pSampler = make_shared<CSampler>();
	pSampler->Begin(PS_TEXTURE_SAMPLER, BIND_PS);
	m_mSampler.insert(pairSampler("Terrain", pSampler));
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Base_Texture.jpg"), NULL, NULL, &pd3dSRV, NULL);
	HeightMapSlot = { PS_SLOT_TERRAIN_BASE };
	HeightMapBindFlag = { BIND_PS };
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/Base_Texture.jpg"), pSampler, HeightMapSlot, HeightMapBindFlag);
	m_mTexture.insert(pairTexture("TerrainBase", pTexture));
	//terrain detail texture
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Detail_Texture_9.jpg"), NULL, NULL, &pd3dSRV, NULL);
	HeightMapSlot = { PS_SLOT_TERRAIN_DETAIL };
	HeightMapBindFlag = { BIND_PS };
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/Detail_Texture_9.jpg"), pSampler, HeightMapSlot, HeightMapBindFlag);
	m_mTexture.insert(pairTexture("TerrainDetail", pTexture));
	//terrain normal map
	//make sampler
	pSampler = make_shared<CSampler>();
	pSampler->Begin(PS_NORMALMAP_SAMPLER, BIND_PS);
	m_mSampler.insert(pairSampler("TerrainNormal", pSampler));
	//make sampler
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Base_Texture_Normal.jpg"), NULL, NULL, &pd3dSRV, NULL);
	HeightMapSlot = { PS_SLOT_NORMALMAP };
	HeightMapBindFlag = { BIND_PS };
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/Base_Texture_Normal.jpg"), pSampler, HeightMapSlot, HeightMapBindFlag);
	m_mTexture.insert(pairTexture("TerrainNormalMap", pTexture));

	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/Test/03_Monster/tex_Zombunny_diffuse.png"), NULL, NULL, &pd3dSRV, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Lower_SP_HEY.tga"), NULL, NULL, &pd3dsrvTexture, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/fbx/CH_PC_ElfF_F_RB0101_A00_Upper_D_HEY.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/Test/02_Character_Juno/tex_Juno_diffuse.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/Test/assuv01-2.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../../Assets/Model/Test/t-unit04-1.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	UINT JunoSlot = { PS_TEXTURE };
	UINT JunoBindFlag = { BIND_PS };
	pTexture = make_shared<CTexture>();
	pTexture->Begin(_T("../../Assets/Model/Test/03_Monster/tex_Zombunny_diffuse.png"), m_mSampler["DEFAULT"], JunoSlot, JunoBindFlag);
	m_mTexture.insert(pairTexture("FBX", pTexture));

}

shared_ptr<CSampler> CResourceManager::CreateSampler(UINT Slot, UINT BindFlag, D3D11_TEXTURE_ADDRESS_MODE Mode, D3D11_FILTER Filter, D3D11_COMPARISON_FUNC ComparisionFunc, float MinLOD, float MaxLOD){
	shared_ptr<CSampler> pSampler;
	pSampler = make_shared<CSampler>();
	pSampler->Begin(Slot, BindFlag, Mode, Filter, ComparisionFunc, MinLOD, MaxLOD);

	return pSampler;
}

shared_ptr<CTexture> CResourceManager::CreateTexture(string name, const TCHAR* pstrTextureNames, shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag){
	//UINT Slot = { PS_SLOT_SKYBOX };
	//UINT BindFlag = { BIND_PS };
	//_TCHAR pstrTextureNames[128];
	shared_ptr<CTexture> pTexture;

	//make sampler
	pTexture = make_shared<CTexture>();
	pTexture->Begin((wchar_t*)pstrTextureNames, pSampler, Slot, BindFlag);
	m_mTexture.insert(pairTexture(name, pTexture));
	return pTexture;

}

void CResourceManager::CreateRenderShaders(){
	//core render shader
	
	shared_ptr<CRenderShader> pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//model
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//color
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//uv
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	pShader->CreateVS(TEXT("VertexShader.cso"), vertexDesc, 7);
	pShader->CreatePS(TEXT("PixelShader.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("Core", pShader));
	//core render shader

	//default
	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSDefault.cso"), vertexDesc, 7);
	pShader->CreatePS(TEXT("PSDefault.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DEFAULT", pShader));
	//default

	//animation render shader
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC AnimationvertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//model
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//color
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//uv
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//bone weight
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//bone index
	};
	pShader->CreateVS(TEXT("VSAnimationObject.cso"), AnimationvertexDesc, 5);
	pShader->CreatePS(TEXT("PSAnimationObject.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("AnimationObject", pShader));
	//animation render shader

	//light shader
	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSDirectionalLight.cso"));
	pShader->CreatePS(TEXT("PSDirectionalLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DirectionalLight", pShader));

	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSPointLight.cso"));
	pShader->CreateHS(TEXT("HSPointLight.cso"));
	pShader->CreateDS(TEXT("DSPointLight.cso"));
	pShader->CreatePS(TEXT("PSPointLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("PointLight", pShader));
	
	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSPointLight.cso"));
	pShader->CreateHS(TEXT("HSPointLight.cso"));
	pShader->CreateDS(TEXT("DSPointLight.cso"));
	pShader->CreatePS(TEXT("PSDebugLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DebugPointLight", pShader));

	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSCapsuleLight.cso"));
	pShader->CreateHS(TEXT("HSCapsuleLight.cso"));
	pShader->CreateDS(TEXT("DSCapsuleLight.cso"));
	pShader->CreatePS(TEXT("PSCapsuleLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("CapsuleLight", pShader));

	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSCapsuleLight.cso"));
	pShader->CreateHS(TEXT("HSCapsuleLight.cso"));
	pShader->CreateDS(TEXT("DSCapsuleLight.cso"));
	pShader->CreatePS(TEXT("PSDebugLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DebugCapsuleLight", pShader));

	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSSpotLight.cso"));
	pShader->CreateHS(TEXT("HSSpotLight.cso"));
	pShader->CreateDS(TEXT("DSSpotLight.cso"));
	pShader->CreatePS(TEXT("PSSpotLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("SpotLight", pShader));

	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSSpotLight.cso"));
	pShader->CreateHS(TEXT("HSSpotLight.cso"));
	pShader->CreateDS(TEXT("DSSpotLight.cso"));
	pShader->CreatePS(TEXT("PSDebugLight.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DebugSpotLight", pShader));
	//light shader

	//post processing shader
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC PostProcessingvertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	pShader->CreateVS(TEXT("VSPostProcessing.cso"), PostProcessingvertexDesc, 2);
	pShader->CreatePS(TEXT("PSPostProcessing.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("PostProcessing", pShader));
	//post processing shader
	//terrain
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC TerrainvertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	pShader->CreateVS(TEXT("VSTerrain.cso"), TerrainvertexDesc, 7);
	pShader->CreateHS(TEXT("HSTerrain.cso"));
	pShader->CreateDS(TEXT("DSTerrain.cso"));
	pShader->CreatePS(TEXT("PSTerrain.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("Terrain", pShader));
	//terrain
	//BoundingBox
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC BoundingBoxDesc[] =
	{
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "EXTEND", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "QUATERNION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	pShader->CreateVS(TEXT("VSBoundingBox.cso"), BoundingBoxDesc, 3);
	pShader->CreateGS(TEXT("GSBoundingBox.cso"));
	pShader->CreatePS(TEXT("PSBoundingBox.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("BoundingBox", pShader));
	//BoundingBox

	//debug textue
	D3D11_INPUT_ELEMENT_DESC DebugTextureDesc[] =
	{
		{ "LEFTTOP", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "RIGHTBOTTOM", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	pShader = make_shared<CRenderShader>();
	pShader->CreateVS(TEXT("VSDebugTexture.cso"), DebugTextureDesc, 2);
	pShader->CreateGS(TEXT("GSDebugTexture.cso"));
	pShader->CreatePS(TEXT("PSDebugTexture.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("DebugTexture", pShader));

	//shader
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC skyboxVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	pShader->CreateVS(TEXT("VSSkyBox.cso"), skyboxVertexDesc, 6);
	pShader->CreatePS(TEXT("PSSkyBox.cso"));
	m_mRenderShader.insert(pairShader("SkyBox", pShader));


	//core render shader
	pShader = make_shared<CRenderShader>();
	D3D11_INPUT_ELEMENT_DESC CoordinateSysvertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },//model
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	pShader->CreateVS(TEXT("VSCoordinateSys.cso"), CoordinateSysvertexDesc, 5);
	pShader->CreatePS(TEXT("PSCoordinateSys.cso"));
	pShader->Begin();
	m_mRenderShader.insert(pairShader("CoordinateSys", pShader));
	//core render shader
}

void CResourceManager::CreateMeshs(){
	//mesh
	shared_ptr<CMesh> pMesh;

	pMesh = make_shared<CDirectionalLightMesh>();
	pMesh->Begin();
	m_mMesh.insert(pairMesh("DirectionalLight", pMesh));
}

//void CResourceManager::CreateAnimater(string path, string animaterName){
//
//}

void CResourceManager::CreateBuffers(){
	//instance buffer
	shared_ptr<CBuffer> pInstanceBuffer;
	shared_ptr<CBuffer> pConstantBuffer;

	UINT BufferStride = { sizeof(VS_VB_INSTANCE) };
	UINT nObject = { 1000 };
	pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);
	m_mBuffer.insert(pairBuffer("DEFAULTIB", pInstanceBuffer));

	nObject = { 50000 };
	pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);
	m_mBuffer.insert(pairBuffer("50000IB", pInstanceBuffer));

	nObject = { 1 };
	pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);
	m_mBuffer.insert(pairBuffer("ONEIB", pInstanceBuffer));

	nObject = { (UINT)SPACE_NUM };
	pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);//space
	m_mBuffer.insert(pairBuffer("SpaceIB", pInstanceBuffer));
	pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);//terrain
	m_mBuffer.insert(pairBuffer("TerrainIB", pInstanceBuffer));
	
	//nObject = { BOUNDINGBOX_NUM };
	//BufferStride = sizeof(VS_VB_BOUNDINGBOX_INSTANCE);
	//pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);//BoundingBox
	//m_mBuffer.insert(pairBuffer("BoundingBoxIB", pInstanceBuffer));
	//nObject = { COORD_NUM };
	//BufferStride = sizeof(VS_VB_INSTANCE);
	//pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);//coordinateSys
	//m_mBuffer.insert(pairBuffer("CoordinateSysIB", pInstanceBuffer));
	////debug texture
	//nObject = { 1 };
	//BufferStride = sizeof(VS_VB_DEBUG_TEXTURE_INSTANCE);
	//pInstanceBuffer = CBuffer::CreateInstancingBuffer(nObject, BufferStride);
	//m_mBuffer.insert(pairBuffer("DebugTextureIB", pInstanceBuffer));
	////instance buffer

	//light buffer
	UINT Slot = { PS_OBJECT_BUFFER_SLOT };
	UINT BindFlag = { BIND_PS };
	BufferStride = { sizeof(DIRECTIONAL_AMBIENT_LIGHT) };
	nObject = { 1 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("DirectionalLightCB", pConstantBuffer));

	/*Slot = { DS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(POINT_LIGHT_DS_CB) };
	BindFlag = { BIND_DS };
	nObject = { 1000 };
	UINT Offset = { 0 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("PointLightCB1", pConstantBuffer));
		
	Slot = { PS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(POINT_LIGHT_PS_CB) };
	BindFlag = { BIND_PS };
	nObject = { 1000 };
	Offset = { 0 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("PointLightCB2", pConstantBuffer));*/

	//test fbx
	/*Slot = { VS_CB_MODEL };
	BufferStride = { sizeof(VS_VB_INSTANCE) };
	BindFlag = { BIND_VS };
	nObject = { 1 };
	Offset = { 0 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("FBX", pConstantBuffer));*/
	////debug
	//Slot = { DS_OBJECT_BUFFER_SLOT };
	//BufferStride = { sizeof(POINT_LIGHT_DS_CB) };
	//BindFlag = { BIND_DS };
	//nObject = { 225 };
	//Offset = { 0 };
	//pConstantBuffer = make_shared<CConstantBuffer>();
	//pConstantBuffer->Begin(nObject, BufferStride, Slot, BindFlag, Offset);
	//m_mBuffer.insert(pairBuffer("DebugPointLightCB1", pConstantBuffer));
	//
	//Slot = { PS_OBJECT_BUFFER_SLOT };
	//BufferStride = { sizeof(POINT_LIGHT_PS_CB) };
	//BindFlag = { BIND_PS };
	//nObject = { 225 };
	//Offset = { 0 };
	//pConstantBuffer = make_shared<CConstantBuffer>();
	//pConstantBuffer->Begin(nObject, BufferStride, Slot, BindFlag, Offset);
	//m_mBuffer.insert(pairBuffer("DebugPointLightCB2", pConstantBuffer));
	//debug

	/*Slot = { DS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(SPOT_LIGHT_DS_CB) };
	BindFlag = { BIND_DS };
	nObject = { 1000 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("SpotLightCB1", pConstantBuffer));
	Slot = { PS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(SPOT_LIGHT_PS_CB) };
	BindFlag = { BIND_PS };
	nObject = { 1000 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("SpotLightCB2", pConstantBuffer));

	Slot = { DS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(CAPSULE_LIGHT_DS_CB) };
	BindFlag = { BIND_DS };
	nObject = { 1000 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("CapsuleLightCB1", pConstantBuffer));
	Slot = { PS_OBJECT_BUFFER_SLOT };
	BufferStride = { sizeof(CAPSULE_LIGHT_PS_CB) };
	BindFlag = { BIND_PS };
	nObject = { 1000 };
	pConstantBuffer = CBuffer::CreateConstantBuffer(nObject, BufferStride, Slot, BindFlag);
	m_mBuffer.insert(pairBuffer("CapsuleLightCB2", pConstantBuffer));*/
}

void CResourceManager::CreateGlobalBuffers()
{
	//shared_ptr<CBuffer> pGlobalBuffer;
	////global buffer
	//UINT Slot = { VS_GLOBAL_BUFFER_SLOT };
	//UINT BufferStride = { sizeof(TERRAIN_GLOBAL_VALUE) };
	//UINT BindFlag = { BIND_VS | BIND_DS };
	//pGlobalBuffer = CBuffer::CreateConstantBuffer(1, BufferStride, Slot, BindFlag);
	//m_mGlobalBuffer.insert(pairBuffer("TerrainGB", pGlobalBuffer));

}

void CResourceManager::CreateMaterials(){

	shared_ptr<CMaterial> pMaterial;
	//material
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 1.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("DEFAULT", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.f, 0.f, 0.f, 1.0f), 1.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("RED", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.5f, 0.1f, 0.1f, 1.0f), 4.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("Core", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.1f, 0.6f, 0.6f, 1.0f), 4.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("Boost", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.1f, 0.7f, 0.0f, 1.0f), 4.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("Weapon", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.6f, 0.5f, 0.1f, 1.0f), 4.0f, 1.0f);
	m_mMaterial.insert(pairMaterial("Adaptor", pMaterial));

	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), 8.0f, 2.0f);
	m_mMaterial.insert(pairMaterial("Plane", pMaterial));

	//terrain
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8.0f, 5.0f);
	m_mMaterial.insert(pairMaterial("Terrain", pMaterial));
	//BoundingBox
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8.0f, 2.0f);
	m_mMaterial.insert(pairMaterial("BoundingBox", pMaterial));
	//fbx
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1.f, 1.f);
	m_mMaterial.insert(pairMaterial("FBX", pMaterial));
	//bunny
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.0f, 1.f, 1.f, 1.f), 1.f, 1.f);
	m_mMaterial.insert(pairMaterial("BUNNY", pMaterial));
	//skybox
	pMaterial = make_shared<CMaterial>();
	pMaterial->Begin(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 1.f, 1.f);
	m_mMaterial.insert(pairMaterial("SkyBox", pMaterial));
}

UINT CResourceManager::CreateMultiMesh(string path, string name) {
	wstring ws{ L"" };
	ws.assign(path.cbegin(), path.cend());
	wstring extention{ PathFindExtension(ws.c_str()) };

	if (L".FBX" == extention || L".fbx" == extention) {
		return CreateFBXResource(path, name);
	}
	else if (L".GJM" == extention || L".gjm" == extention || L".txt" == extention) {
		return CreateGJMResource(path, name);
	}
}
//void CResourceManager::CreateAnimaters(){
//
//}

void CResourceManager::CreateGJMResources(){
	//WCHAR*pwchar;
	//IMPORTER->ReadWCHAR(pwchar, 0);
}

UINT CResourceManager::CreateGJMResource(string path, string name){
	//IMPORTER->Begin(path);
	//char pName[20];

	//bool bHasAnimation = IMPORTER->ReadBool();

	//shared_ptr<CMesh> pMesh;
	//UINT nMeshCnt = IMPORTER->ReadUINT();
	//for (UINT i = 0; i < nMeshCnt; ++i) {
	//	sprintf(pName, "%s%d", name.c_str(), i);
	//	pMesh = CFileBasedMesh::CreateMeshFromGJMFile(i, bHasAnimation);
	//	m_mMesh.insert(pairMesh(pName, pMesh));
	//}
	//if (false == bHasAnimation) return nMeshCnt;

	////animater
	//sprintf(pName, "%s", name.c_str());
	//shared_ptr<CAnimater> pAnimater = CAnimater::CreateAnimaterFromGJMFile();
	//m_mAnimater.insert(pairAnimater(pName, pAnimater));

	////animation info
	//int animationCnt = IMPORTER->ReadInt();
	//for (int i = 0; i < animationCnt; ++i) {
	//	CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromGJMFile(pAnimater);
	//}//end animation info for

	//IMPORTER->End();
	//return nMeshCnt;
	return 0;
}

void CResourceManager::CreateFBXResources(){

}

UINT CResourceManager::CreateFBXResource(string path, string name){
	//string sPath{ "" };
	//sPath.assign(path.cbegin(), path.cend());

	////multi mesh data load
	//FBXIMPORTER->Begin(sPath);
	//char pName[20];

	//shared_ptr<CFileBasedMesh> pFBXMesh;
	////	int i = FBXIMPORTER->GetMeshCnt();
	//if (FBXIMPORTER->GetHasAnimation()) {

	//	for (UINT i = 0; i < FBXIMPORTER->GetMeshCnt(); ++i) {
	//		sprintf(pName, "%s%d", name.c_str(), i);
	//		pFBXMesh = CFileBasedMesh::CreateMeshFromFBXFile(i);
	//		m_mMesh.insert(pairMesh(pName, pFBXMesh));
	//	}

	//	sprintf(pName, "%s", name.c_str());
	//	shared_ptr<CAnimater> pAnimater = CAnimater::CreateAnimaterFromFBXFile();
	//	CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromFBXFile(pAnimater);
	//	m_mAnimater.insert(pairAnimater(pName, pAnimater));
	//}
	//else {
	//	for (UINT j = 0; j < FBXIMPORTER->GetMeshCnt(); ++j) {
	//		sprintf(pName, "%s%d", name.c_str(), j);
	//		pFBXMesh = CFileBasedMesh::CreateMeshFromFBXFile(j);
	//		m_mMesh.insert(pairMesh(pName, pFBXMesh));
	//	}
	//}

	//int meshCnt = FBXIMPORTER->GetMeshCnt();
	//FBXIMPORTER->End();
	//return meshCnt;
	return 0;
}

void CResourceManager::ReleaseTextures(){
	for (auto data : m_mSampler) {
		if(data.second)data.second->End();
	}
	for (auto data : m_mTexture) {
		if (data.second)data.second->End();
	}
	m_mTexture.clear();
}

void CResourceManager::ReleaseRenderShaders(){
	for (auto data : m_mRenderShader) {
		if (data.second)data.second->End();
	}
	m_mRenderShader.clear();
}

void CResourceManager::ReleaseMeshs(){
	for (auto data : m_mMesh) {
		if (data.second)data.second->End();
	}
	m_mMesh.clear();
}

void CResourceManager::ReleaseMesh(string name){
	map<string, shared_ptr<CMesh>> ::iterator iter = m_mMesh.find(name);
	(iter->second)->End();
	//delete iter->second.get();
	m_mMesh.erase(iter);
}

//void CResourceManager::ReleaseAnimater(string name){
//	//map<string, shared_ptr<CAnimater>> ::iterator iter = m_mAnimater.find(name);
//	//(iter->second)->End();
//	//m_mAnimater.erase(iter);
//}

void CResourceManager::ReleaseBuffers(){
	for (auto data : m_mBuffer) {
		if (data.second)data.second->End();
	}
	m_mBuffer.clear();
}

void CResourceManager::ReleaseGlobalBuffers(){
	for (auto data : m_mGlobalBuffer) {
		if (data.second)data.second->End();
	}
	m_mGlobalBuffer.clear();
}

void CResourceManager::ReleaseMaterials(){
	for (auto data : m_mMaterial) {
		if (data.second)data.second->End();
	}
	m_mMaterial.clear();
}

//void CResourceManager::ReleaseAnimaters(){
//	for (auto data : m_mAnimater) {
//		if (data.second)data.second->End();
//	}
//	m_mAnimater.clear();
//}

CResourceManager::CResourceManager() :CSingleTonBase<CResourceManager>("resourcemanager") {

}

CResourceManager::~CResourceManager() {

}
