#pragma once
#include "Shader.h"

enum eInputElement {
	IE_POSITION = 0b000001,
	IE_NORMAL = 0b000010,
	IE_TEXCOORD = 0b000100,
	IE_INSTANCEPOS = 0b001000,
	IE_BONEINDEX = 0b010000,
	IE_BONEWEIGHT = 0b100000
};
class CRenderShader : public CShader{
public:
	//----------------------------dxobject-----------------------------
	bool Begin();
	bool End();


	void SetShaderState();
	void CleanShaderState();
	//----------------------------dxobject-----------------------------

	//---------------------------------shader---------------------------
	virtual void UpdateShaderState();
	virtual void ExcuteShaderState() {};//나중 cs를 위한 함수

										//begin func
	virtual bool CreateShaderValues() { return true; } //texture, constbuffer etc.
	bool CreateVS(LPCTSTR name, D3D11_INPUT_ELEMENT_DESC* vsDesc = nullptr, UINT nElement = 0);

	bool CreateGS(LPCTSTR name);
	
	bool CreateHS(LPCTSTR name);
	bool CreateDS(LPCTSTR name);

	bool CreatePS(LPCTSTR name);
	//begin func
	//---------------------------------shader---------------------------

	void SetShaderData(LPCTSTR vsName, D3D11_INPUT_ELEMENT_DESC* vsDesc, UINT nElement, LPCTSTR gsName, LPCTSTR hsName,
		LPCTSTR dsName, LPCTSTR psName);

	static shared_ptr<CRenderShader> CreateRenderShader(UINT BindFlag, UINT InputElementFlag, LPCTSTR ShaderName);
	static shared_ptr<CRenderShader> CreateRenderShader(UINT BindFlag, UINT InputElementFlag, LPCTSTR* ShaderNames);
	//static shared_ptr<CRenderShader> CreateRenderShader(UINT BindFlag, D3D11_INPUT_ELEMENT_DESC* InputElementDesc, LPCTSTR ShaderName);
	//static shared_ptr<CRenderShader> CreateRenderShader(UINT BindFlag, D3D11_INPUT_ELEMENT_DESC* InputElementDesc, LPCTSTR* ShaderNames);
protected:
	void CreateInputLayout(UINT InputElementFlag);

	//-------------------------------shaders-----------------------------
	ID3D11VertexShader* m_vertexShader{ nullptr };
	ID3D11InputLayout* m_inputLayout{ nullptr };

	ID3D11GeometryShader* m_geometryShader{ nullptr };
	ID3D11HullShader* m_hullShader{ nullptr };
	ID3D11DomainShader* m_domainShader{ nullptr };
	
	ID3D11PixelShader* m_pixelShader{ nullptr };
	//-------------------------------shaders-----------------------------


public:
	CRenderShader();
	virtual ~CRenderShader();
};
