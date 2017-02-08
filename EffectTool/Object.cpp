#include "stdafx.h"
#include "Object.h"


bool CMyObject::Begin() {
	m_name.clear();
	m_tag = tag::TAG_DEFAULT;

	return true;
}
bool CMyObject::End() {

	return true;
}
CMyObject::CMyObject(string name, tag t) : m_name(name), m_tag(t) {

}
CMyObject::~CMyObject() {

}