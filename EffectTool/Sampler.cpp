#include "stdafx.h"
#include "Sampler.h"

bool CSampler::Begin(UINT Slot, UINT BindFlag, D3D11_TEXTURE_ADDRESS_MODE Mode, D3D11_FILTER Filter, D3D11_COMPARISON_FUNC ComparisionFunc, float MinLOD, float MaxLOD) {


	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	m_SamplerStartSlot = Slot;
	m_BindFlag = BindFlag;

	//sampler
	d3dSamplerDesc.AddressU = Mode;
	d3dSamplerDesc.AddressV = Mode;
	d3dSamplerDesc.AddressW = Mode;

	d3dSamplerDesc.Filter = Filter;
	d3dSamplerDesc.ComparisonFunc = ComparisionFunc;
	d3dSamplerDesc.MinLOD = MinLOD;
	d3dSamplerDesc.MaxLOD = MaxLOD;

	GLOBALVALUEMGR->GetDevice()->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerState);
	
	return true;
}
bool CSampler::End() {

	if (m_pd3dSamplerState) {
		m_pd3dSamplerState->Release();
	}

	return true;
}

void CSampler::SetShaderState() {

		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
		}
	
}
void CSampler::CleanShaderState() {
	ID3D11SamplerState* pSamplerState[1] = { nullptr };

		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
		}

}

void CSampler::UpdateShaderState() {

}


CSampler::CSampler() : DXObject("sampler") { }
CSampler::~CSampler() { };

