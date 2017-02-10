#include "stdafx.h"
#include "RenderShader.h"


bool CRenderShader::Begin() {
	
	if (false == CreateShaderValues()) return false;

	return true;
}
bool CRenderShader::End() {
	if (m_vertexShader)			m_vertexShader->Release();
	if (m_geometryShader)		m_geometryShader->Release();
	if (m_hullShader)			m_hullShader->Release();
	if (m_domainShader)			m_domainShader->Release();
	if (m_pixelShader)			m_pixelShader->Release();

	if (m_inputLayout)			m_inputLayout->Release();

	return true;
}

void CRenderShader::SetShaderState() {
	// Input Layout
	GLOBALVALUEMGR->GetDeviceContext()->IASetInputLayout(m_inputLayout);

	// vs
	if (m_vertexShader) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetShader(m_geometryShader, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetShader(m_hullShader, nullptr, 0);
	}

	//ds 
	if (m_domainShader) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetShader(m_domainShader, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);
	}
}
void CRenderShader::CleanShaderState() {
	// ps
	if (m_vertexShader) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	}

	// gs
	if (m_geometryShader) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	}

	// hs
	if (m_hullShader) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	}

	//ds
	if (m_domainShader) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	}

	// ps
	if (m_pixelShader) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	}
}

void CRenderShader::UpdateShaderState() {

}

bool CRenderShader::CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateVertexShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_vertexShader
		);

		if (vsDesc) {
			GLOBALVALUEMGR->GetDevice()->CreateInputLayout(
				vsDesc
				, nElement
				, &(loadShader[0])
				, loadShader.size()
				, &m_inputLayout
			);
		}
	}

	if (m_vertexShader) return true;

	return false;
}

bool CRenderShader::CreateGS(LPCTSTR name){

	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateGeometryShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_geometryShader
		);

	}

	if (m_geometryShader) return true;

	return false;
}

bool CRenderShader::CreateHS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateHullShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_hullShader
		);

	}

	if (m_hullShader) return true;

	return false;
}

bool CRenderShader::CreateDS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreateDomainShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_domainShader
		);

	}

	if (m_domainShader) return true;

	return false;
}

bool CRenderShader::CreatePS(LPCTSTR name){
	auto loadShader = ReadCSOFile(name);
	{
		GLOBALVALUEMGR->GetDevice()->CreatePixelShader(
			&(loadShader[0])
			, loadShader.size()
			, nullptr
			, &m_pixelShader
		);

	}

	if (m_pixelShader) return true;

	return false;
}

void CRenderShader::SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName, LPCTSTR dsName, LPCTSTR psName){
	if(vsName) CreateVS(vsName, vsDesc, nElement);
	if(gsName) CreateGS(gsName);
	if(dsName) CreateDS(dsName);
	if(hsName) CreateHS(hsName);
	if(psName) CreatePS(psName);
}

shared_ptr<CRenderShader> CRenderShader::CreateRenderShader(UINT BindFlag, UINT InputElementFlag, LPCTSTR ShaderName){

	return shared_ptr<CRenderShader>();
}

shared_ptr<CRenderShader> CRenderShader::CreateRenderShader(UINT BindFlag, UINT InputElementFlag, LPCTSTR * ShaderNames){

	return shared_ptr<CRenderShader>();
}

//shared_ptr<CRenderShader> CRenderShader::CreateRenderShader(UINT BindFlag, D3D11_INPUT_ELEMENT_DESC * InputElementDesc, LPCTSTR ShaderNames){
//	shared_ptr<CRenderShader> pRenderShader = make_shared<CRenderShader>();
//	WCHAR shaderName[256];
//	UINT shaderIndex{ 0 };
//	if (BindFlag & BIND_VS) {
//		wprintf(shaderName, "VS%s", ShaderNames[shaderIndex++]);
//		pRenderShader->CreateVS(shaderName, InputElementDesc, )
//	}
//	if (BindFlag& BIND_HS) {
//		vDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (BindFlag& BIND_DS) {
//		vDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (BindFlag& BIND_GS) {
//		vDesc.push_back({ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (BindFlag& BIND_PS) {
//		vDesc.push_back({ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (BindFlag& BIND_CS) {
//		vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, 0, D3D11_INPUT_PER_VERTEX_DATA, 1 });
//	}
//
//	return pRenderShader;
//}
//
//void CRenderShader::CreateInputLayout(UINT InputElementFlag){
//	vector<D3D11_INPUT_ELEMENT_DESC> vDesc;
//	UINT nElement{ 0 };
//	if (InputElementFlag & IE_POSITION) {
//		vDesc.push_back( { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } );
//	}
//	if (InputElementFlag& IE_NORMAL) {
//		vDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (InputElementFlag& IE_TEXCOORD) {
//		vDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (InputElementFlag& IE_BONEWEIGHT) {
//		vDesc.push_back({ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (InputElementFlag& IE_BONEINDEX) {
//		vDesc.push_back({ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nElement++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//	}
//	if (InputElementFlag& IE_INSTANCEPOS) {
//		vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, 0, D3D11_INPUT_PER_VERTEX_DATA, 1 });
//		vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 1 });
//		vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 1 });
//		vDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nElement++, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 1 });
//	}
//}


CRenderShader::CRenderShader() : CShader() {

}
CRenderShader::~CRenderShader() {

}