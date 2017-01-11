#include "stdafx.h"
#include "Buffer.h"


void* CBuffer::Map() {

	m_pd3dDeviceContext->Map(m_pd3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedData);
	m_pData = m_MappedData.pData;

	return m_pData;
}

void CBuffer::Unmap() {

	m_pd3dDeviceContext->Unmap(m_pd3dBuffer, 0);
}

CBuffer::~CBuffer()
{
}

