#include "stdafx.h"
#include "DirectionalLight.h"

bool CDirectionalLight::Begin(DIRECTIONAL_AMBIENT_LIGHT& light_info) {
	m_lightid = light_id::LIGHT_DIRECTIONAL;
	//m_objectID = object_id::OBJECT_DIRECTIONAL_LIGHT;

	m_Directional_AmbientData = light_info;

	return CLight::Begin();
}
bool CDirectionalLight::End() {

	return CLight::End();
}
//instance buffer controll base
//void CDirectionalLight::SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera) {
//	
//	//형변환
//	DIRECTIONAL_AMBIENT_LIGHT *pnInstances = (DIRECTIONAL_AMBIENT_LIGHT *)ppMappedResources[0];
//	//transpose 이후 정보 주입
//
//	//directional
//	pnInstances[nInstance].m_DirLightColor = m_Directional_AmbientData.m_DirLightColor;
//	pnInstances[nInstance].m_DirToLight = m_Directional_AmbientData.m_DirToLight;
//	pnInstances[nInstance].m_DirLightPower = m_Directional_AmbientData.m_DirLightPower;
//
//	//ambient
//	pnInstances[nInstance].m_AmbientColor = m_Directional_AmbientData.m_AmbientColor;
//	pnInstances[nInstance].m_AmbientDown = m_Directional_AmbientData.m_AmbientDown;
//	pnInstances[nInstance].m_AmbientRange = m_Directional_AmbientData.m_AmbientRange;
//	pnInstances[nInstance].m_AmbientUp = m_Directional_AmbientData.m_AmbientUp;
//}

CDirectionalLight::CDirectionalLight() {
	
}
CDirectionalLight::~CDirectionalLight() {

}