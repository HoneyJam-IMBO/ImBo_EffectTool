// MyFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "MyFormView.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW)
	, m_TabWnd(NULL)
	, m_iTabIndex(0)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_ctrlTab);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CMyFormView::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CMyFormView �����Դϴ�.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView �޽��� ó�����Դϴ�.


void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_ctrlTab.InsertItem(0, L"Effect");
	m_ctrlTab.InsertItem(1, L"Particle");


	m_EffectDig.Create(IDD_EFFECTDIG, &m_ctrlTab);
	m_ParticleDig.Create(IDD_PARTICLEDIG, &m_ctrlTab);

	CRect	rcWnd;

	m_ctrlTab.GetWindowRect(&rcWnd);

	m_EffectDig.SetWindowPos(NULL, 3, 24, rcWnd.Width() - 8, rcWnd.Height() - 32, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_ParticleDig.SetWindowPos(NULL, 3, 24, rcWnd.Width() - 8, rcWnd.Height() - 32, SWP_NOZORDER);

	m_TabWnd = &m_EffectDig;
}


void CMyFormView::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (m_TabWnd != NULL)
	{
		m_TabWnd->ShowWindow(SW_HIDE);
		m_TabWnd = NULL;
	}

	m_iTabIndex = m_ctrlTab.GetCurSel();

	switch (m_iTabIndex)
	{
	case 0:
		m_EffectDig.ShowWindow(SW_SHOW);
		m_TabWnd = &m_EffectDig;
		break;
	case 1:
		m_ParticleDig.ShowWindow(SW_SHOW);
		m_TabWnd = &m_ParticleDig;
		break;
	}
	*pResult = 0;
}
