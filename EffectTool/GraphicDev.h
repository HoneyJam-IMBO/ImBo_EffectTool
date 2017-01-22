#pragma once

#include "stdafx.h"
#include "IncludeHeader.h"
#include "Sampler.h"
#include "Texture.h"

class CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)
public:
	CGraphicDev();
	~CGraphicDev();

public:
	RECT	m_rcClient;
	ID3D11Device*			m_pd3dDevice{ nullptr };
	ID3D11DeviceContext*	m_pd3dDeviceContext{ nullptr };

	IDXGISwapChain*			m_pdxgiSwapChain{ nullptr };
	ID3D11RenderTargetView*	m_pd3dRenderTargetView{ nullptr };

	ID3D11Texture2D*		m_pd3dDepthStencilBuffer{ nullptr };
	ID3D11DepthStencilView*	m_pd3dDepthStencilView{ nullptr };

	//---------------------------deferred rendering-----------------------
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };//0
	ID3D11Texture2D			 *m_pd3dtxtNormal{ nullptr };//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow{ nullptr };//2


	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal{ nullptr };//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow{ nullptr };//2

	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt{ nullptr };//0
	ID3D11RenderTargetView   *m_pd3drtvNormal{ nullptr };//1
	ID3D11RenderTargetView   *m_pd3drtvSpecPow{ nullptr };//2

	vector<shared_ptr<CTexture>> m_vObjectLayerResultTexture;
	vector<shared_ptr<CTexture>> m_vLightLayerResultTexture;

	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvLight{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvLight{ nullptr };
	//--------------------------light render target----------------

public:
	void InitDevice();


private:
	bool CreateD3D11Deivce();
	bool CreateRenderTargetView();
	void ReleaseForwardRenderTargets();
};

