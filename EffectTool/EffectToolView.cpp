
// EffectToolView.cpp : CEffectToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEffectToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CEffectToolView ����/�Ҹ�

CEffectToolView::CEffectToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

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
	//------------------------------------------ī�޶� ����--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		60.0f * XM_PI / 180.0f
		, float(m_pGraphicDev->GetrcClient().right) / float(m_pGraphicDev->GetrcClient().bottom)
		, 0.01f, 10000.0f);

	//��ġ ����

	//viewprojection��� ����
	XMVECTOR eye = { 0.0f, 58.f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 0.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS�� ����Ʈ ����
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

// CEffectToolView �׸���

void CEffectToolView::OnDraw(CDC* /*pDC*/)
{
	CEffectToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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

// CEffectToolView �μ�
void CEffectToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEffectToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CEffectToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CEffectToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CEffectToolView ����

#ifdef _DEBUG
void CEffectToolView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEffectToolDoc* CEffectToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEffectToolDoc)));
	return (CEffectToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CEffectToolView �޽��� ó����


