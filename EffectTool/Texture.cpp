#include "stdafx.h"
#include "Texture.h"

//array
bool CTexture::Begin(UINT nTextures, _TCHAR(*ppstrFilePaths)[128], shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer)
{
	//sampler
	m_pSampler = pSampler;
	//constant buffer
	m_pConstantBuffer = pConstantBuffer;

	//texture
	m_TextureStartSlot = Slot;
	m_pd3dsrvTexture = CreateTexture2DArraySRV(ppstrFilePaths, nTextures);
	m_BindFlag = BindFlag;

	return true;
}
bool CTexture::Begin(UINT nTextures, ID3D11Texture2D **ppd3dTextures, shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer)
{
	//sampler
	m_pSampler = pSampler;
	//constant buffer
	m_pConstantBuffer = pConstantBuffer;

	//texture
	m_TextureStartSlot = Slot;
	m_pd3dsrvTexture = CreateTexture2DArraySRV(ppd3dTextures, nTextures);
	m_BindFlag = BindFlag;

	return true;
}

//from file
bool CTexture::Begin(_TCHAR(pstrFilePath)[128], shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer){
	wstring path{ pstrFilePath };
	m_sPath.assign(path.cbegin(), path.cend());

	//sampler
	m_pSampler = pSampler;
	//constant buffer
	m_pConstantBuffer = pConstantBuffer;

	//texture
	m_TextureStartSlot = Slot;
	D3DX11CreateShaderResourceViewFromFile(GLOBALVALUEMGR->GetDevice(), pstrFilePath, NULL, NULL, &m_pd3dsrvTexture, NULL);
	m_BindFlag = BindFlag;
	
	
	return true;
}
//from srv
bool CTexture::Begin(ID3D11ShaderResourceView* pShaderResourceView, shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer) {
	//sampler
	m_pSampler = pSampler;
	//constant buffer
	m_pConstantBuffer = pConstantBuffer;

	//texture
	//srv가 이미 만들어 져 있다면 끝. array로 제작 불가
	m_TextureStartSlot = Slot;
	m_pd3dsrvTexture = pShaderResourceView;
	m_BindFlag = BindFlag;

	return true;
}

bool CTexture::End() {
	//sampler
	//if (m_pSampler) {
	//	m_pSampler->End();
	//}
	//constantbuffer
	if (m_pConstantBuffer) {
		m_pConstantBuffer->End();
	}

	//texture
		if (m_pd3dsrvTexture) m_pd3dsrvTexture->Release();

	return true;
}

void CTexture::SetShaderState() {
	//sampler
	if(m_pSampler) m_pSampler->SetShaderState();
	//constant buffer
	if(m_pConstantBuffer) m_pConstantBuffer->SetShaderState();

	//texture
		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvTexture);
		}
}
void CTexture::CleanShaderState() {
	//sampler
	if(m_pSampler) m_pSampler->CleanShaderState();
	//constant buffer
	if(m_pConstantBuffer) m_pConstantBuffer->CleanShaderState();

	//texture
	ID3D11ShaderResourceView* pSRVs[1] = { nullptr };
		if (m_BindFlag & BIND_VS) {
			GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_DS) {
			GLOBALVALUEMGR->GetDeviceContext()->DSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_HS) {
			GLOBALVALUEMGR->GetDeviceContext()->HSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_GS) {
			GLOBALVALUEMGR->GetDeviceContext()->GSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_PS) {
			GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}
		if (m_BindFlag & BIND_CS) {
			GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(m_TextureStartSlot, 1, pSRVs);
		}

}

void CTexture::UpdateShaderState() {
	//update texture matrix
	if (m_pConstantBuffer) m_pConstantBuffer->UpdateShaderState();
}



void CTexture::SetSampler(shared_ptr<CSampler> pSampler){
	if (!pSampler) return;
	m_pSampler = pSampler;
}
void CTexture::SetConstantBuffer(shared_ptr<CBuffer> pConstantBuffer) {
	if (!pConstantBuffer) return;
	m_pConstantBuffer = pConstantBuffer;
}

ID3D11ShaderResourceView* CTexture::CreateTexture2DArraySRV(_TCHAR(*ppstrFilePaths)[128], UINT nTextures){
	
	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
	for (UINT i = 0; i < nTextures; i++) D3DX11CreateTextureFromFile(GLOBALVALUEMGR->GetDevice(), ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
	//? map 왜 함? 병신임?
			GLOBALVALUEMGR->GetDeviceContext()->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			GLOBALVALUEMGR->GetDeviceContext()->Unmap(ppd3dTextures[t], m);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	return(pd3dsrvTextureArray);
}

ID3D11ShaderResourceView* CTexture::CreateTexture2DArraySRV(ID3D11Texture2D **ppd3dTextures, UINT nTextures) {

	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; //DXGI_FORMAT_R8G8B8A8_UNORM
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;
	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			//? map 왜 함? 병신임?
			GLOBALVALUEMGR->GetDeviceContext()->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			GLOBALVALUEMGR->GetDeviceContext()->Unmap(ppd3dTextures[t], m);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	return(pd3dsrvTextureArray);
}

CTexture::CTexture() : DXObject("texture") { }
CTexture::~CTexture() { };

