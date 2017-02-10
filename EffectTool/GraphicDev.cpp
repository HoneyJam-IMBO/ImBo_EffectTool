#include "stdafx.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev()
{
	m_pd3dtxtColorSpecInt= nullptr ;//0
	m_pd3dtxtNormal= nullptr ;//1
	m_pd3dtxtSpecPow= nullptr;//2


	m_pd3dsrvColorSpecInt= nullptr ;//0
	m_pd3dsrvNormal= nullptr ;//1
	m_pd3dsrvSpecPow= nullptr;//2

	m_pd3drtvColorSpecInt= nullptr ;//0
	m_pd3drtvNormal= nullptr ;//1
	m_pd3drtvSpecPow= nullptr ;//2
}

CGraphicDev::~CGraphicDev()
{
}

void CGraphicDev::InitDevice()
{
	GetClientRect(g_hWnd, &m_rcClient);
	CreateD3D11Deivce();
	CreateRenderTargetView();
}

bool CGraphicDev::CreateD3D11Deivce()
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
		dxgiSwapChainDesc.BufferDesc.Width = m_rcClient.right;
		// Height : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Height = m_rcClient.bottom;
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
	dxgiSwapChainDesc.OutputWindow = g_hWnd;

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

#pragma endregion


	// �����ϴ� �ϵ���� �׷��� ����̹��� �����մϴ�.
	D3D_DRIVER_TYPE d3dDriverTypes[]
	{
		D3D_DRIVER_TYPE_HARDWARE
		, D3D_DRIVER_TYPE_WARP
		, D3D_DRIVER_TYPE_REFERENCE
	};

	// �� �迭�� �� ���� ���α׷����� �����ϴ� DirectX �ϵ���� ��� ���� ������ �����մϴ�.
	// ������ �����ؾ� �մϴ�.
	// ������ ���� ���α׷��� �ʿ��� �ּ� ��� ������ �����ؾ� �մϴ�.
	// ������ �������� ���� ��� ��� ���� ���α׷��� 9.1�� �����ϴ� ������ ���ֵ˴ϴ�.
	D3D_FEATURE_LEVEL d3dFeatureLevels[]
	{
		 D3D_FEATURE_LEVEL_11_0
		, D3D_FEATURE_LEVEL_10_1
		, D3D_FEATURE_LEVEL_10_0
	};
	// ���� ������ ��� �ϵ���� ��� ���� ���� ���� ���մϴ�.
	auto szFeatureLevel = static_cast<UINT>(GetArraySize(d3dFeatureLevels));

	// ���������� ���� ��ġ�� ��� ������ �����ϱ� ���� �����Դϴ�.
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;

	// �� �÷��״� API �⺻���� �ٸ� �� ä�� ������ ǥ�鿡 ���� ������
	// �߰��մϴ�. Direct2D���� ȣȯ���� ���� �ʿ��մϴ�.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)

	auto IsSDKLayerAvailable = []() -> bool {
		return SUCCEEDED(D3D11CreateDevice(nullptr
			, D3D_DRIVER_TYPE_NULL			// ���� �ϵ���� ��ġ�� ���� �ʿ䰡 �����ϴ�.
			, 0
			, D3D11_CREATE_DEVICE_DEBUG		// SDK ���̾ Ȯ���ϼ���.
			, nullptr						// ��� ��� ������ ����˴ϴ�.
			, 0
			, D3D11_SDK_VERSION				// Windows ����� ���� ��� �׻� �� ���� D3D11_SDK_VERSION���� �����մϴ�.
			, nullptr						// D3D ��ġ ������ ������ �ʿ䰡 �����ϴ�.
			, nullptr						// ��� ������ �� �ʿ䰡 �����ϴ�.
			, nullptr						// D3D ��ġ ���ؽ�Ʈ ������ ������ �ʿ䰡 �����ϴ�.
		));
	};

	// SDK ���̾� ������ Ȯ���ϼ���.
	if (IsSDKLayerAvailable())
	{
		// ������Ʈ�� ����� ���� ���� ��쿡�� �� �÷��װ� �ִ� SDK ���̾ ���� ������� ����Ͻʽÿ�.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	for (D3D_DRIVER_TYPE &p : d3dDriverTypes)
	{
		if (SUCCEEDED(hResult = D3D11CreateDevice(NULL						// �⺻ ����͸� ����Ϸ��� nullptr�� �����մϴ�.
			, p							// �ϵ���� �׷��� ����̹��� ����Ͽ� ��ġ�� ����ϴ�.
			, 0							// ����̹��� D3D_DRIVER_TYPE_SOFTWARE�� �ƴ� ��� 0�̾�� �մϴ�.
			, creationFlags				// ����� �� Direct2D ȣȯ�� �÷��׸� �����մϴ�.
			, d3dFeatureLevels			// �� ���� ���α׷��� ������ �� �ִ� ��� ���� ����Դϴ�.
			, szFeatureLevel			// �� ����� ũ���Դϴ�.
			, D3D11_SDK_VERSION			// Windows ����� ���� ��� �׻� �� ���� D3D11_SDK_VERSION���� �����մϴ�.
			, &m_pd3dDevice				// ������� Direct3D ��ġ�� ��ȯ�մϴ�.
			, &nd3dFeatureLevel			// ������� ��ġ�� ��� ������ ��ȯ�մϴ�.
			, &m_pd3dDeviceContext		// ��ġ ���� ���ؽ�Ʈ�� ��ȯ�մϴ�.
		)
		)
			)
			break;
	}

	// ��� ���� ������ ����̹����� ������ �����ϸ� ���α׷��� �����մϴ�.
	if (!m_pd3dDevice)
	{
		MessageBox(g_hWnd, TEXT("���� ������ �׷��� ����� �����ϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}

	// DXGI Device �� �޽��ϴ�.
	//IDXGIDevice3 *pdxgiDevice = NULL;
	//if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice3), (LPVOID*)&pdxgiDevice)))
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
	{
		MessageBox(g_hWnd, TEXT("DXGI Device ��ü�� ��ȯ���� ���߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	// DXGI Factory �ν��Ͻ��� DXGIFactory���� �޽��ϴ�.
	//IDXGIFactory *pdxgiFactory = NULL;
	IDXGIFactory *pdxgiFactory = NULL;

	UINT udxgiFlag = 0;
#ifdef _DEBUG
	
#endif
	//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
	if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
	{
		MessageBox(g_hWnd, TEXT("DXGIFactory������ ��ü ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	//  SwapChain �� �����մϴ�
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
	{
		MessageBox(g_hWnd, TEXT("SwapChain �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
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

bool CGraphicDev::CreateRenderTargetView()
{
	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;

	// render target �� �����ϱ� ���� back buffer �� SwapChain ���� ��û�մϴ�.
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };

	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	// ��ȯ���� ���۸� ����Ͽ� render target view �� �����մϴ�.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);

	// back buffer �� ��ȯ�մϴ�.
	if (pd3dBackBuffer) pd3dBackBuffer->Release();



	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	{
		// depth stencil "texture" �� �����մϴ�.
		D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;

		// �޸𸮴� 0���� �ʱ�ȭ�մϴ�.
		ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));



		// Width : texture �� �ʺ��Դϴ�.
		d3dDepthStencilBufferDesc.Width = m_rcClient.right;
		// Height : texture �� �����Դϴ�.
		d3dDepthStencilBufferDesc.Height = m_rcClient.bottom;
		// MipLevels : texture �ִ� MipMap Level ��. 
		//				���� ���ø� �ؽ�ó : 1
		//				�ִ� �Ӹ� ���� : 0
		d3dDepthStencilBufferDesc.MipLevels = 1;
		// ArraySize :texture �迭�� texture ����. (�迭�� �ƴϸ� 1)
		d3dDepthStencilBufferDesc.ArraySize = 1;
		// Format : texture �ȼ� ����
		d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// SampleDesc : ���� ���ø��� ǰ���� ����
		// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
		{

			// Count : �ȼ� �� ���� ����
			//	1  : ���� ���ø��� ���� ����
			//	2~ : �ش� ����ŭ�� ���� ���ø�
			d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
			// Quality : ǰ�� ����
			// 0 : ���� ���ø��� ���� ����
			d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
		}

		// Usage : texture �� �а� ���� ����� ���� ����
		d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		// BindFlags : ���������� �ܰ� ��� ������ ������ ����
		d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		// MiscFlags : ���ҽ��� ���� �߰� ���� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.MiscFlags = 0;
		// CPUAccessFlags : CPU�� ���۸� ����� �� �ִ� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.CPUAccessFlags = 0;

		//	�⺻ ����-���ٽ� ������ ���� ���۸� �������� �Ѵ�.
		if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;

		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);


	}
	{//create framework texture
		ReleaseForwardRenderTargets();
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
		d3dTexture2DDesc.Width = m_rcClient.right;
		d3dTexture2DDesc.Height = m_rcClient.bottom;
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
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene0 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene0 RTV Create-----------------------------------------//

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//�ڱ� texture set -> sampler set����

		//---------------------make texture---------------------
		//texture set to light rendercontainer
		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvColorSpecInt };
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS };
		//make sampler
		shared_ptr<CSampler> pSampler = make_shared<CSampler>();
		pSampler->Begin();
		shared_ptr<CTexture> pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 1 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 2 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>();
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture����
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

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
		///m_pPostProcessingLayer->SetFirstPassData(m_rcClient.right, m_rcClient.bottom);
		///m_pPostProcessingLayer->SetBloomThreshold(2.0f);
		float fMiddleGrey = 0.0025f;
		float fWhite = 1.5f;
		float fBloomScale = 0.1f;
		///m_pPostProcessingLayer->SetFinalPassData(fMiddleGrey, fWhite, fBloomScale);
		//post processing layer
	}

	//	DXGI_FRAME_STATISTICS p;
	//	m_pdxgiSwapChain->GetFrameStatistics(&p);


	// Direct2D : RenderTarget���� 2DBackBuffer�� ȹ���մϴ�.
	//return(CreateD2DBackBuffer());

	return true;
}

void CGraphicDev::ReleaseForwardRenderTargets()
{
	m_vObjectLayerResultTexture.clear();
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;
}
