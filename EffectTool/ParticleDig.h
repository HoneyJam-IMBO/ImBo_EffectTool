#pragma once


// CParticleDig ��ȭ �����Դϴ�.

class CParticleDig : public CDialog
{
	DECLARE_DYNAMIC(CParticleDig)

public:
	CParticleDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CParticleDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTICLEDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnCancel() {}
	virtual void OnOK();
};
