
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

	int k = 0;

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


