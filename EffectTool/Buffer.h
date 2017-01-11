#pragma once
class CBuffer
{
public:
	bool Begin() { return true; };
	virtual bool End() { return true; };

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};

	//map
	void* Map();
	void Unmap();
	//unmap

	//getter
	UINT GetBufferStride() { return m_BufferStride; }
	UINT GetnObject() { return m_nObject; }
	UINT GetOffset() { return m_Offset; }
	ID3D11Buffer* GetBuffer() { return m_pd3dBuffer; }

protected:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pd3dDeviceContext;

protected:

	//buffer 1°³
	ID3D11Buffer*					m_pd3dBuffer{ nullptr };

	//map data 1°³
	D3D11_MAPPED_SUBRESOURCE		m_MappedData{ nullptr };
	void*							m_pData{ nullptr };

	//buffe info 1°³
	UINT							m_nObject{ 0 };
	UINT							m_BufferStride{ 0 };
	UINT							m_ByteWidth{ 0 };
	UINT							m_Offset{ 0 };
public:
	CBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext)
		: m_pd3dDevice(pd3dDevice)
		, m_pd3dDeviceContext(pd3dDeviceContext) {};
	virtual ~CBuffer();

};

