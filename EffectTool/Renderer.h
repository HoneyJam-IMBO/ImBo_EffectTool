#pragma once

#include "SingleTon.h"
//layers
#include "LightLayer.h"
#include "ObjectLayer.h"
#include "PostProcessingLayer.h"

struct TDownscaleCB {
	UINT nWidth;//수평/2
	UINT nHeight;//수직/2
	float fHorResRcp;
	float fVerResRcp;
	XMFLOAT4 ProjParams;
	XMFLOAT4X4 ViewMtx;
	float fOffsetRadius;
	float fRadius;
	float fMaxDepth;
	UINT pad;
};

class CRenderer : public CSingleTonBase<CRenderer> {

public:
	bool Begin();
	bool End();

	void Render(shared_ptr<CCamera> pCamera);
	void Update(float fTimeElapsed);

	void ObjectRender(shared_ptr<CCamera> pCamera);
	void LightRender(shared_ptr<CCamera> pCamera);
	void PostProcessing(shared_ptr<CCamera> pCamera);

	void SetForwardRenderTargets();
	void SetMainRenderTargetView();
	void SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil);

	bool CreateSwapChain();
	bool CreateRenderTargetView();
	void ReleaseForwardRenderTargets();
	void ReleaseAmbientOcculutionViews();
	void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView);

	bool ResizeBuffer();
private:
	IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };
	ID3D11RenderTargetView	*	m_pd3dRenderTargetView{ nullptr };
	
	ID3D11DepthStencilState* m_pd3dDepthStencilState{ nullptr };
	// Rendering 추가 부분 : Depth-Stencil 을 하지 않으면 정상적인 큐브가 출력되지 않는다.
	ID3D11DepthStencilView		*m_pd3ddsvReadOnlyDepthStencil{ nullptr };

	//---------------------------deferred rendering----------------------
	ID3D11Texture2D			 *m_pd3dtxtDepthStencil{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };//0
	ID3D11Texture2D			 *m_pd3dtxtNormal{ nullptr };//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow{ nullptr };//2


	ID3D11ShaderResourceView *m_pd3dsrvDepthStencil{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal{ nullptr };//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow{ nullptr };//2

	ID3D11DepthStencilView	 *m_pd3ddsvDepthStencil{ nullptr };
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
	//-------------------------layer-------------------------
	CObjectLayer* m_pObjectLayer{ nullptr };
	CLightLayer* m_pLightLayer{ nullptr };
	CPostProcessingLayer* m_pPostProcessingLayer{ nullptr };
	//-------------------------layer-------------------------

	//--------------------------ao----------------------------
	CComputeShader* m_pCSAONormalDepthDownScale;
	ID3D11Buffer				*m_pd3dbufAOMiniNormalDepth{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAOMiniNormalDepth{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAOMiniNormalDepth{ nullptr };

	CComputeShader* m_pCSSSAOCompute;
	ID3D11Texture2D				*m_pd3dtxtAmbientOcculution{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAmbientOcculution{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAmbientOcculution{ nullptr };

	//CStaticBuffer				*m_pAmbientOcculutionConstantBuffer{ nullptr };
	TDownscaleCB				*m_pTDoownscaleCB{ nullptr };
	ID3D11Buffer				*m_pd3dbufAOConstantBuffer{ nullptr };
	shared_ptr<CTexture> m_pAmbientOcculutionTexture;
	//--------------------------ao----------------------------
public:
	CRenderer();
	virtual ~CRenderer();
};
