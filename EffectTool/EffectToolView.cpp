
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

	m_pDirectionalLight = new CDirectionalLight;
	m_pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
		XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f),XMFLOAT4(0.0, 0.1, 0.1, 0.0) , XMFLOAT4(1.5f, 1.5f, 1.5f, 1),//dir
		XMFLOAT4(0.1, 0.1, 0.1, 1), XMFLOAT4(0.1f, 0.1f, 0.1f, 1), XMFLOAT4(0.1f, 0.1f, 0.1f, 1), XMFLOAT4(5.1f, 5.1f, 5.1f, 1)//ambient
	});
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

	m_pGraphicDev->m_pd3dDeviceContext->ClearDepthStencilView(m_pGraphicDev->m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pGraphicDev->m_pd3drtvColorSpecInt, m_pGraphicDev->m_pd3drtvNormal, m_pGraphicDev->m_pd3drtvSpecPow };
	float fClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	if (m_pGraphicDev->m_pd3drtvColorSpecInt)
		m_pGraphicDev->m_pd3dDeviceContext->ClearRenderTargetView(m_pGraphicDev->m_pd3drtvColorSpecInt, fClearColor);
	if (m_pGraphicDev->m_pd3drtvNormal)
		m_pGraphicDev->m_pd3dDeviceContext->ClearRenderTargetView(m_pGraphicDev->m_pd3drtvNormal, fClearColor);
	if (m_pGraphicDev->m_pd3drtvSpecPow)
		m_pGraphicDev->m_pd3dDeviceContext->ClearRenderTargetView(m_pGraphicDev->m_pd3drtvSpecPow, fClearColor);

	m_pGraphicDev->m_pd3dDeviceContext->OMSetRenderTargets(RENDER_TARGET_NUMBER, pd3dRTVs, m_pGraphicDev->m_pd3dDepthStencilView);


	//Light
	m_pGraphicDev->m_pd3dDeviceContext->ClearDepthStencilView(m_pGraphicDev->m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pGraphicDev->m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pGraphicDev->m_pd3drtvLight, m_pGraphicDev->m_pd3dDepthStencilView);

	for (auto texture : m_pGraphicDev->m_vObjectLayerResultTexture) {
		texture->SetShaderState();
	}
	//m_stackScene.top()->LightRender();


	for (auto texture : m_pGraphicDev->m_vObjectLayerResultTexture) {
		texture->CleanShaderState();

	}

	int k = 0;

	m_pGraphicDev->m_pdxgiSwapChain->Present(0, 0);
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


