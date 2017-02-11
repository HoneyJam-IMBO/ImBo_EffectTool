
#include "stdafx.h"
#include "Renderer.h"

bool CRenderer::Begin() {
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = stencilMarkOp;
	descDepth.BackFace = stencilMarkOp;
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDepthStencilState);
	if (false == CreateSwapChain()) return false;

	//layer
	m_pPostProcessingLayer = new CPostProcessingLayer();
	m_pPostProcessingLayer->Begin();

	m_pObjectLayer = new CObjectLayer();
	m_pObjectLayer->Begin();

	m_pLightLayer = new CLightLayer();
	m_pLightLayer->Begin();
	//layer

	// render target�� depth-stencil buffer ����/ deferred texture ����
	if (!CreateRenderTargetView()) {
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("RenderTarget�̳� Depth-Stencil ���� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return false;
	}
	return true;
}

bool CRenderer::End() {
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();
	
	if(m_pTDoownscaleCB) delete m_pTDoownscaleCB;
	if (m_pd3dbufAOConstantBuffer)m_pd3dbufAOConstantBuffer->Release();
	m_pd3dbufAOConstantBuffer = nullptr;

	ReleaseForwardRenderTargets();
	ReleaseAmbientOcculutionViews();

	//layer
	if (m_pPostProcessingLayer) {
		m_pPostProcessingLayer->End();
		delete m_pPostProcessingLayer;
	}
	if (m_pObjectLayer) {
		m_pObjectLayer->End();
		delete m_pObjectLayer;
	}
	if (m_pLightLayer) {
		m_pLightLayer->End();
		delete m_pLightLayer;
	}
	return true;
}
 
void CRenderer::Render(shared_ptr<CCamera> pCamera) {
	// Store the previous depth state
	ID3D11DepthStencilState* pPrevDepthState;
	UINT nPrevStencil;
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&pPrevDepthState, &nPrevStencil);

	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	SetForwardRenderTargets();//gbuff�� �� rtv/ dsv set
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);
	//object render
	ObjectRender(pCamera);

	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->SetShaderState();
	}
	
	//clear
	ID3D11UnorderedAccessView* pUAVs[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pUAVs, (UINT*)(&pUAVs));
	LightRender(pCamera);
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->CleanShaderState();
	}
	
	//ClearDepthStencilView(m_pd3dDepthStencilView);

	//��¥ rtv set! rtv�� set�ϱ� dsv�� set���� �ʴ´�. 
	SetMainRenderTargetView();

	for (auto texture : m_vLightLayerResultTexture) {
		texture->SetShaderState();
	}
	PostProcessing(pCamera);


	for (auto texture : m_vLightLayerResultTexture) {
		texture->CleanShaderState();
	}

	//present
	m_pdxgiSwapChain->Present(0, 0);

	// Restore the previous depth state
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(pPrevDepthState, nPrevStencil);
	if (pPrevDepthState) pPrevDepthState->Release();
}
void CRenderer::Update(float fTimeElapsed) {
	m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);
}

void CRenderer::ObjectRender(shared_ptr<CCamera> pCamera) {
	//object layer render
	m_pObjectLayer->Render(pCamera);

	////debuge
	//if (INPUTMGR->GetDebugMode())
	//	DEBUGER->DebugRender(pCamera);

}
void CRenderer::LightRender(shared_ptr<CCamera> pCamera) {
	//light layer render
	m_pLightLayer->Render(pCamera);
}
void CRenderer::PostProcessing(shared_ptr<CCamera> pCamera) {
	//rtv�� Ǯ��ũ�� ��ο� 
	m_pPostProcessingLayer->Render(pCamera);
}
void CRenderer::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView) {
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CRenderer::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[3] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xo4lor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 1.f, 1.f };
	if (m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(3, pd3dRTVs, m_pd3ddsvDepthStencil);
}
void CRenderer::SetMainRenderTargetView() {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, nullptr);
}
void CRenderer::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil) {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(nRenderTarget, pd3dRTVs, pd3ddsvDepthStencil);
}


bool CRenderer::CreateSwapChain()
{
	// Swap Chain Description ����ü
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
#pragma region [DXGI_SWAP_CHAIN_DESC �ʱ�ȭ]
	// ����ü ����
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	// BufferCount : �ĸ� ������ ���� ����
	dxgiSwapChainDesc.BufferCount = 2;

	// BufferDesc : �ĸ� ������ ���÷��� ������ ����
	{
		// Width : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
		// Height : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
		// Format : �ĸ� ���� �ȼ� ����
		/// DirectX 11-1(Chap 01)-Device, p.49 ����
		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// RefreshRate : ȭ�� ���� ������ Hz ������ ����
		{
			// Denominator : �и�
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			// Numerator : ����
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		}
		// ScanlineOrdering : scan line �׸��� ��� ����(�⺻ 0)
		//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        (0) : ��ĵ ���� ������ �������� ����	
		//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        (1) : ���α׷��ú� ��ĵ ���� ���� ����
		//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  (2) : ���� �ʵ�� �̹��� ����
		//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  (3) : ���� �ʵ�� �̹��� ����
		dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}

	// BufferUsage : �ĸ� ���ۿ� ���� ǥ�� ��� ��İ� CPU�� ���� ��� ����
	//	DXGI_USAGE_SHADER_INPUT				: shader �� �Է����� ���
	//	DXGI_USAGE_RENDER_TARGET_OUTPUT		: render target���� ���
	//	DXGI_USAGE_BACK_BUFFER         		: back buffer �� ���
	//	DXGI_USAGE_SHARED              		: ���� ����
	//	DXGI_USAGE_READ_ONLY           		: �б� ����
	//	DXGI_USAGE_DISCARD_ON_PRESENT  		: DXGI ���� ���� ���(����ڰ� ������� ����)
	//	DXGI_USAGE_UNORDERED_ACCESS    		: ������ȭ ����
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// OutputWindow : ��� �������� �ڵ��� ����(�ݵ�� ��ȿ�ؾ� ��)
	dxgiSwapChainDesc.OutputWindow = GLOBALVALUEMGR->GethWnd();

	// SampleDesc : ���� ���ø��� ǰ���� ����
	// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
	{
		// Count : �ȼ� �� ���� ����
		//	1  : ���� ���ø��� ���� ����
		//	2~ : �ش� ����ŭ�� ���� ���ø�
		dxgiSwapChainDesc.SampleDesc.Count = 1;
		// Quality : ǰ�� ����
		// 0 : ���� ���ø��� ���� ����
		dxgiSwapChainDesc.SampleDesc.Quality = 0;
	}

	// Windowed : ������ ��� �Ǵ� ��ü ȭ�� ��� ���� ~ TRUE  | â ��� 
	//												   ~ FALSE | ��ü ȭ��
	dxgiSwapChainDesc.Windowed = TRUE;

	// Flags : Swap Chain ���ۿ� ���� ���� ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_NONPREROTATED		(1) : ��ü ȭ�� ��忡�� ���� ������ ������ ȭ������ �ű� �� �ڵ� ȸ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	(2) : ���� ���α׷��� ���÷��� ��带 ������ �� ����
	//	DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE		(4) : ���� ���α׷��� GDI �� ������ �� �� ����. �ĸ� ���ۿ� GetDC() ��� ����
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*2*/;

	// SwapEffect : Swaping�� ó���ϴ� ���û����� ����(�⺻ : 0)
	//	DXGI_SWAP_EFFECT_DISCARD		(0) : ���� ������ ���
	//	DXGI_SWAP_EFFECT_SEQUENTIAL		(1) : ���� ����
	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	(2) : Flip ���� ����
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGI Device ��ü�� ��ȯ���� ���߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	// DXGI Factory �ν��Ͻ��� DXGIFactory���� �޽��ϴ�.
	//IDXGIFactory *pdxgiFactory = NULL;
	IDXGIFactory *pdxgiFactory = NULL;

	UINT udxgiFlag = 0;
//#ifdef _DEBUG
//	udxgiFlag |= DXGI_CREATE_FACTORY_DEBUG;
//#endif
	//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
	if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGIFactory������ ��ü ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	//  SwapChain �� �����մϴ�
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("SwapChain �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}

	//	// Direct2D : Direct2D �ν��Ͻ��� �����մϴ�.
	//	if (!CreateD2D1Device(pdxgiDevice))
	//	{
	//		MessageBox(m_hWnd, TEXT("Direct2D �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
	//		return(false);
	//	}

	// �Ҵ���� COM ��ü�� ��ȯ�մϴ�.
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
	return true;
}

bool CRenderer::CreateRenderTargetView() {

	HRESULT hResult = S_OK;
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };
	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();
	{
		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels = 1;
		//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//----------------------------------------Resource Desc-----------------------------------------//
		//----------------------------------------TextUre Desc-----------------------------------------//
		D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
		::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTexture2DDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
		d3dTexture2DDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
		d3dTexture2DDesc.MipLevels = 1;
		d3dTexture2DDesc.ArraySize = 1;
		d3dTexture2DDesc.SampleDesc.Count = 1;
		d3dTexture2DDesc.SampleDesc.Quality = 0;
		d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DDesc.CPUAccessFlags = 0;
		d3dTexture2DDesc.MiscFlags = 0;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		//----------------------------------------TextUre Desc-----------------------------------------//
		//----------------------------------------Render Desc-----------------------------------------//
		D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		ReleaseForwardRenderTargets();
		ReleaseAmbientOcculutionViews();

		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		d3dTexture2DDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDepthStencil, &d3dSRVDesc, &m_pd3dsrvDepthStencil);
		//real depth stencil
		d3dDepthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;
		if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvReadOnlyDepthStencil))) return(false);
		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//--------------------------------------Scene3 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene3 RTV Create-----------------------------------------//

		//�ڱ� texture set -> sampler set����
		//---------------------make texture---------------------
		//texture set to light rendercontainer
		//make sampler
		shared_ptr<CSampler> pSampler = RESOURCEMGR->GetSampler("DEFAULT");

		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvDepthStencil};
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS | BIND_CS };
		shared_ptr<CTexture> pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvColorSpecInt };
		Slot = { 1 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 2 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		
		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 3 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture����
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		pTexture = make_shared<CTexture>();
		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		//make sampler
		shared_ptr<CSampler> pLightTexSampler = make_shared<CSampler>();
		UINT LightTexSamplerBindFlag = { BIND_PS | BIND_CS };
		UINT LightTexSamplerSlot = { 0 };
		pLightTexSampler->Begin(LightTexSamplerSlot, LightTexSamplerBindFlag);
		pTexture->Begin(m_pd3dsrvLight, pLightTexSampler, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.push_back(pTexture);
		//light texture����

		//first pass data set
		m_pPostProcessingLayer->SetFirstPassData(GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom);
		m_pPostProcessingLayer->SetBloomThreshold(2.0f);
		float fMiddleGrey = 0.0025f;
		float fWhite = 1.5f;
		float fBloomScale = 0.1f;
		m_pPostProcessingLayer->SetFinalPassData(fMiddleGrey, fWhite, fBloomScale);
		//post processing layer
	}
	{
	UINT nWidth = GLOBALVALUEMGR->GetrcClient().right / 2;
	UINT nHeight = GLOBALVALUEMGR->GetrcClient().bottom / 2;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate SSAO
	D3D11_TEXTURE2D_DESC t2dDesc = {
		nWidth, //UINT Width;
		nHeight, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R32_TYPELESS,//DXGI_FORMAT_R8_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pd3dtxtAmbientOcculution);

	// Create the UAVs
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(UAVDesc));
	UAVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtAmbientOcculution, &UAVDesc, &m_pd3duavAmbientOcculution);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtAmbientOcculution, &SRVDesc, &m_pd3dsrvAmbientOcculution);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scaled depth buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.StructureByteStride = 4 * sizeof(float);
	bufferDesc.ByteWidth = nWidth * nHeight * bufferDesc.StructureByteStride;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pd3dbufAOMiniNormalDepth);
	//t2dDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pd3dtxtAOMiniNormalDepth);

	ZeroMemory(&UAVDesc, sizeof(UAVDesc));
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.NumElements = nWidth * nHeight;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dbufAOMiniNormalDepth, &UAVDesc, &m_pd3duavAOMiniNormalDepth);

	//ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.FirstElement = 0;
	SRVDesc.Buffer.NumElements = nWidth * nHeight;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dbufAOMiniNormalDepth, &SRVDesc, &m_pd3dsrvAOMiniNormalDepth);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate down scale depth constant buffer
	D3D11_BUFFER_DESC CBDesc;
	ZeroMemory(&CBDesc, sizeof(CBDesc));
	CBDesc.Usage = D3D11_USAGE_DYNAMIC;
	CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CBDesc.ByteWidth = sizeof(TDownscaleCB);
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&CBDesc, NULL, &m_pd3dbufAOConstantBuffer);

	}
	return true;
}

void CRenderer::ReleaseForwardRenderTargets() {
	//texture end
	m_vObjectLayerResultTexture.clear();
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dtxtDepthStencil) m_pd3dtxtDepthStencil->Release();
	m_pd3dtxtDepthStencil = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3dsrvDepthStencil) m_pd3dsrvDepthStencil->Release();
	m_pd3dsrvDepthStencil = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3ddsvDepthStencil) m_pd3ddsvDepthStencil->Release();
	m_pd3ddsvDepthStencil = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;


}

void CRenderer::ReleaseAmbientOcculutionViews(){
	
	if(m_pd3dbufAOMiniNormalDepth)m_pd3dbufAOMiniNormalDepth->Release();//1/4scale texture
	m_pd3dbufAOMiniNormalDepth = nullptr;
	//if (m_pd3dtxtAOMiniNormalDepth)m_pd3dtxtAOMiniNormalDepth->Release();//1/4scale texture
	//m_pd3dtxtAOMiniNormalDepth = nullptr;
	if(m_pd3dsrvAOMiniNormalDepth) m_pd3dsrvAOMiniNormalDepth->Release();
	m_pd3dsrvAOMiniNormalDepth = nullptr;

	if(m_pd3duavAOMiniNormalDepth)m_pd3duavAOMiniNormalDepth->Release();
	m_pd3duavAOMiniNormalDepth = nullptr;
	
	if(m_pd3dtxtAmbientOcculution)m_pd3dtxtAmbientOcculution->Release();//1/4scale texture
	m_pd3dtxtAmbientOcculution = nullptr;
	if(m_pd3dsrvAmbientOcculution)m_pd3dsrvAmbientOcculution->Release();
	m_pd3dsrvAmbientOcculution = nullptr;
	if(m_pd3duavAmbientOcculution)m_pd3duavAmbientOcculution->Release();
	m_pd3duavAmbientOcculution = nullptr;
}

bool CRenderer::ResizeBuffer() {
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();

	if (FAILED(m_pdxgiSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0)))
		MessageBox(nullptr, TEXT(""), TEXT(""), MB_OK);

	//resize rtv size, deferred texture size
	CreateRenderTargetView();

	return true;
}

CRenderer::CRenderer() :CSingleTonBase<CRenderer>("rendereringleton") {

}

CRenderer::~CRenderer() {

}
