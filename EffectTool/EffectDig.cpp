// EffectDig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectDig.h"
#include "afxdialogex.h"


// CEffectDig 대화 상자입니다.

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


// CEffectDig 메시지 처리기입니다.
