
// EffectToolView.cpp : CEffectToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EffectTool.h"
#endif

#include "EffectToolDoc.h"
#include "EffectToolView.h"
#include "MainFrm.h"
#include "RenderThread.h"
#include "GraphicDev.h"
#include "DirectionalLight.h"
#include "SceneMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
// CEffectToolView

IMPLEMENT_DYNCREATE(CEffectToolView, CView)

BEGIN_MESSAGE_MAP(CEffectToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEffectToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CEffectToolView 생성/소멸

CEffectToolView::CEffectToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEffectToolView::~CEffectToolView()
{
	CloseHandle(m_LoopEvent);
	CloseHandle(m_RenderEvent);
	delete m_pThread;
	m_pGraphicDev->DestroyInstance();

	RESOURCEMGR->End();
	RCSELLER->End();
	RENDERER->End();
	INPUTMGR->End();
}

BOOL CEffectToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CEffectToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// RenderThread /////////////////////////////////////
	m_LoopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_LoopEvent == NULL)		return;
	m_RenderEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (m_RenderEvent == NULL)	return;

	m_pThread = new CRenderThread;
	m_pThread->m_LoopEvent = &m_LoopEvent;
	m_pThread->m_RenderEvent = &m_RenderEvent;
	m_pThread->Initialize();
	/////////////////////////////////////////////////////
	g_hWnd = m_hWnd;

	m_pGraphicDev = CGraphicDev::GetInstance();
	m_pGraphicDev->InitDevice();
	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	RENDERER->Begin();
	INPUTMGR->Begin();


	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		60.0f * XM_PI / 180.0f
		, float(m_pGraphicDev->GetrcClient().right) / float(m_pGraphicDev->GetrcClient().bottom)
		, 0.01f, 10000.0f);

	//위치 조정

	//viewprojection행렬 제작
	XMVECTOR eye = { 0.0f, 58.f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 0.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS에 뷰포트 연결
	m_pCamera->SetViewport(0, 0, m_pGraphicDev->GetrcClient().right, m_pGraphicDev->GetrcClient().bottom, 0.0f, 1.0f);


	m_pScene = new CSceneMain(m_pCamera.get());
	m_pScene->Begin();

	//framework.Begin(hInst, hWnd);
	//framework.ChangeScene(new CSceneMain(&framework));

	//m_pDirectionalLight = new CDirectionalLight;
	//m_pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
	//	XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f),XMFLOAT4(0.0, 0.1, 0.1, 0.0) , XMFLOAT4(1.5f, 1.5f, 1.5f, 1),//dir
	//	XMFLOAT4(0.1, 0.1, 0.1, 1), XMFLOAT4(0.1f, 0.1f, 0.1f, 1), XMFLOAT4(0.1f, 0.1f, 0.1f, 1), XMFLOAT4(5.1f, 5.1f, 5.1f, 1)//ambient
	//});
}

// CEffectToolView 그리기

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	WaitForSingleObject(m_RenderEvent, INFINITE);
	TIMEMGR->Tick();
	float fDeltaTime = TIMEMGR->GetTimeElapsed();
	//Update
	RENDERER->Update(fDeltaTime);
	m_pCamera->Update(fDeltaTime);
	m_pScene->Animate(fDeltaTime);

	//Render
	m_pCamera->SetShaderState();
	RENDERER->Render(m_pCamera);

	//m_pGraphicDev->m_pdxgiSwapChain->Present(0, 0);
	SetEvent(m_LoopEvent);
}

// CEffectToolView 인쇄
void CEffectToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CEffectToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEffectToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEffectToolView 진단

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView 메시지 처리기


