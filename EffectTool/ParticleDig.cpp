// ParticleDig.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "ParticleDig.h"
#include "afxdialogex.h"


// CParticleDig ��ȭ �����Դϴ�.

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


// CParticleDig �޽��� ó�����Դϴ�.

