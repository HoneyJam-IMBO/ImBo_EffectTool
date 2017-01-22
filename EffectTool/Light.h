#pragma once
//#include "GameObject.h"

enum light_id {
	//LIGHT_AMBIENT, ambient light 정보는 DIRECTIONAL mesh안에 들어있다.
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
	//light는 버퍼를 따로 가지지 않는다.
	//rendercontainer가 가질 예정
	light_id m_lightid{ light_id::LIGHT_END };

public:
	CLight(){ m_lightid = light_id::LIGHT_END; }
	virtual ~CLight() {}
};
