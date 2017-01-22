#pragma once
//#include "GameObject.h"

enum light_id {
	//LIGHT_AMBIENT, ambient light ������ DIRECTIONAL mesh�ȿ� ����ִ�.
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_CAPSULE,
	LIGHT_END
};

class CLight {
public:
	bool Begin();
	virtual bool End() { return true; };

	//instance buffer controll
	//virtual void SetBufferInfo(void** ppMappedResource, int& nInstance, shared_ptr<CCamera> pCamera);
	
	light_id GetLightID() { return m_lightid; };

protected:
	//light�� ���۸� ���� ������ �ʴ´�.
	//rendercontainer�� ���� ����
	light_id m_lightid{ light_id::LIGHT_END };

public:
	CLight(){ m_lightid = light_id::LIGHT_END; }
	virtual ~CLight() {}
};
