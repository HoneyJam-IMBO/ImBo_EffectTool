// EffectDig.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectDig.h"
#include "afxdialogex.h"


// CEffectDig ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEffectDig, CDialog)

CEffectDig::CEffectDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EFFECTDIG, pParent)
{

}

CEffectDig::~CEffectDig()
{
}

void CEffectDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CEffectDig::OnOK()
{
	//CDialog::OnOK();
	return;
}


BEGIN_MESSAGE_MAP(CEffectDig, CDialog)
END_MESSAGE_MAP()


// CEffectDig �޽��� ó�����Դϴ�.
