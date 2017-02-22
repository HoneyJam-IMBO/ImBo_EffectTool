#pragma once


// CEffectDig 대화 상자입니다.

class CEffectDig : public CDialog
{
	DECLARE_DYNAMIC(CEffectDig)

public:
	CEffectDig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectDig();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnCancel() {}
	virtual void OnOK();
};
