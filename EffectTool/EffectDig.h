#pragma once


// CEffectDig ��ȭ �����Դϴ�.

class CEffectDig : public CDialog
{
	DECLARE_DYNAMIC(CEffectDig)

public:
	CEffectDig(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffectDig();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTDIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnCancel() {}
	virtual void OnOK();
};
