#include "stdafx.h"
#include "RenderContainerSeller.h"


bool CRenderContainerSeller::Begin(){
	
	return true;
}

bool CRenderContainerSeller::End(){

	//render container delete
	for (auto RenderContainer : m_mRenderContainer) {
		RenderContainer.second->End();
		delete RenderContainer.second;
	}
	m_mRenderContainer.clear();
	//render container delete

	return true;
}

CRenderContainer* CRenderContainerSeller::GetRenderContainer(object_id objectid) {
	//CRenderContainer* pContainer{ nullptr };
	//int nMaxObjects{ 0 };

	//있으면 바로 return
	if (m_mRenderContainer[objectid]) return m_mRenderContainer[objectid];

	//수정되면 여기 수정 + container factory 하나 더 만듬
	switch (objectid) {
	case object_id::OBJECT_TEST:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("Rect5"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("Core"));
		m_mRenderContainer[objectid]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
		//m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("DEFAULT"));

		m_mRenderContainer[objectid]->Begin();

		//pContainer->CreateSharedBuffer(nMaxObjects, sizeof(VS_VB_INSTANCE));
		break;
	case object_id::OBJECT_PLANE:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("Plane"));//mesh set과 동시에 instancing buffer 생성 및 set
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("Plane"));
		m_mRenderContainer[objectid]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
		//m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("DEFAULT"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_DIRECTIONAL_LIGHT:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight"));//mesh set과 동시에 instancing buffer 생성 및 se
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("DirectionalLight"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("DirectionalLightCB"));

		m_mRenderContainer[objectid]->Begin();
		break;

	case object_id::OBJECT_POINT_LIGHT:
		//point light
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("PointLight"));
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("PointLight"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_SPOT_LIGHT:
		//point light
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("SpotLight"));
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("SpotLight"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_CAPSULE_LIGHT:
		//point light
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight"));
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("CapsuleLight"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));

		m_mRenderContainer[objectid]->Begin();
		break;

	case object_id::OBJECT_POSTPROCESSING:
		//자기 shader set
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("PostProcessing"));
		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("PostProcessing"));
		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_SKYBOX:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("SkyBox"));
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("SkyBox"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("SkyBox"));
		m_mRenderContainer[objectid]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("SkyBox"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_SPACE:
		//adaptor
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("Space"));//mesh set과 동시에 instancing buffer 생성 및 set
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("Adaptor"));
		m_mRenderContainer[objectid]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("SpaceIB"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_TERRAIN:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->SetMesh(RESOURCEMGR->GetMesh("Terrain"));
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("Terrain"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
		m_mRenderContainer[objectid]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("TerrainIB"));
		m_mRenderContainer[objectid]->AddGlobalBuffer(RESOURCEMGR->GetGlobalBuffer("TerrainGB"));
		//m_mRenderContainer[objectid]->AddTexture(m_pSkyBoxTexture);
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("TerrainHeightMap"));
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("TerrainBase"));
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("TerrainDetail"));
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("TerrainNormalMap"));

		m_mRenderContainer[objectid]->Begin();
		break;
	
	case object_id::OBJECT_FBX_ELF:
		m_mRenderContainer[objectid] = new CRenderContainer();

		m_mRenderContainer[objectid]->AddMesh(RESOURCEMGR->GetMesh("Rect1"));

		////mesh set
		//char pName[20];
		//for (int i = 0; i < 10; ++i) {
		//	sprintf(pName, "FBX_%d", i);
		//	m_mRenderContainer[objectid]->AddMesh(RESOURCEMGR->GetMesh(pName));
		//}
		//mesh set
		//m_mRenderContainer[objectid]->SetMesh(m_pTestMultiMesh);

		//m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("Core"));

#ifdef USE_ANIM
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
#else
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
#endif
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("FBX"));
		//m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("FBX"));
		//m_mRenderContainer[objectid]->SetAnimater(RESOURCEMGR->GetAnimater("ELF"));

		m_mRenderContainer[objectid]->Begin();
		break;
	case object_id::OBJECT_FBX_BUNNY:
		m_mRenderContainer[objectid] = new CRenderContainer();
		
		m_mRenderContainer[objectid]->AddMesh(RESOURCEMGR->GetMesh("BUNNY"));
#ifdef USE_ANIM
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
#else
		m_mRenderContainer[objectid]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
#endif
		m_mRenderContainer[objectid]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
		m_mRenderContainer[objectid]->AddTexture(RESOURCEMGR->GetTexture("FBX"));
		m_mRenderContainer[objectid]->AddMaterial(RESOURCEMGR->GetMaterial("BUNNY"));
//		m_mRenderContainer[objectid]->SetAnimater(RESOURCEMGR->GetAnimater("BUNNY"));

		m_mRenderContainer[objectid]->Begin();
		break;
	}

	return m_mRenderContainer[objectid];
}
//
//void CRenderContainerSeller::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
//	static UINT currMesh = 0;
//	static char name[20];
//	switch (nMessageID)
//	{
//	case WM_KEYDOWN:
//
//		switch (wParam)
//		{
//		case VK_F5:
//			sprintf(name, "FBX_%d", ++currMesh);
//			m_mRenderContainer[OBJECT_FBX_ELF]->SetMesh(RESOURCEMGR->GetMesh(name));
//			break;
//		case VK_F6:
//			sprintf(name, "FBX_%d", --currMesh);
//			m_mRenderContainer[OBJECT_FBX_ELF]->SetMesh(RESOURCEMGR->GetMesh(name));
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//}
