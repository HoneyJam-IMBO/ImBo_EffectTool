#include "stdafx.h"
#include "RenderThread.h"
#include "MainFrm.h"
#include <process.h>

#include "EffectToolView.h"
#include "TimeMgr.h"

CRenderThread::CRenderThread()
{
}


CRenderThread::~CRenderThread()
{
}

void CRenderThread::Initialize()
{
	m_pToolView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
	CTimeMgr::GetInstance()->InitTime();
	HANDLE hThread = CreateThread(NULL, 0, FuncThread, this, 0, NULL);
}

DWORD WINAPI CRenderThread::FuncThread(LPVOID arg)
{
	CRenderThread*	pMyThread = (CRenderThread*)arg;
	while(true)
	{
		WaitForSingleObject(*(pMyThread->m_LoopEvent), INFINITE);
		while (true)
		{
			float fTime = CTimeMgr::GetInstance()->GetTime();
			pMyThread->m_fAccTime += fTime;

			CTimeMgr::GetInstance()->SetTime();
			if (pMyThread->m_fAccTime > 1.f / 30.f)
			{
				SetEvent(*(pMyThread->m_RenderEvent));
				pMyThread->m_pToolView->m_fTimeDelta = fTime;
				pMyThread->m_pToolView->Invalidate(FALSE);
				break;
			}
		}
	}
	return 0;
}
