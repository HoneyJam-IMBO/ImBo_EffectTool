#pragma once

class CEffectToolView;
class CRenderThread
{
public:
	CRenderThread();
	~CRenderThread();

public:
	HANDLE*		m_LoopEvent;
	HANDLE*		m_RenderEvent;
	CEffectToolView*		m_pToolView;

	float		m_fAccTime{ 0.f };

public:
	void	Initialize();
	static DWORD WINAPI FuncThread(LPVOID arg);
};

