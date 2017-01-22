#pragma once
#include "Light.h"

struct DIRECTIONAL_AMBIENT_LIGHT {
	XMFLOAT4 m_DirToLight;
	XMFLOAT4 m_DirLightColor;
	XMFLOAT4 m_DirLightPower;

	XMFLOAT4 m_AmbientDown;
	XMFLOAT4 m_AmbientRange;
	XMFLOAT4 m_AmbientUp;
	XMFLOAT4 m_AmbientColor;
};

class CDirectionalLight : public CLight {
public:
	bool Begin(DIRECTIONAL_AMBIENT_LIGHT& light_info);
	virtual bool End();

	//instance buffer controll
	//virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);

private:
	//data
	DIRECTIONAL_AMBIENT_LIGHT m_Directional_AmbientData;
	//data

public:
	CDirectionalLight();
	virtual ~CDirectionalLight();
};