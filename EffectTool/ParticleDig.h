#pragma once


// CParticleDig 대화 상자입니다.

class CParticleDig : public CDialog
{
	DECLARE_DYNAMIC(CParticleDig)

public:
	CParticleDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CParticleDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARTICLEDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnCancel() {}
	virtual void OnOK();
};
