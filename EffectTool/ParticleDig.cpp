// ParticleDig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "ParticleDig.h"
#include "afxdialogex.h"


// CParticleDig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CParticleDig, CDialog)

CParticleDig::CParticleDig(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PARTICLEDIG, pParent)
{

}

CParticleDig::~CParticleDig()
{
}

void CParticleDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CParticleDig::OnOK()
{
	//CDialog::OnOK();
	return;
}

BEGIN_MESSAGE_MAP(CParticleDig, CDialog)
END_MESSAGE_MAP()


// CParticleDig 메시지 처리기입니다.

