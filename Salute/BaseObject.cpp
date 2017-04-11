#include "StdAfx.h"

#pragma region CBaseObject

CBaseObject::CBaseObject() : m_bActive(true) {
}

CBaseObject::~CBaseObject() {
  m_bActive = false;
}

void CBaseObject::SetActive(bool bActive) {
  m_bActive = bActive;
}

void CBaseObject::Update() {
  if (!IsActive()) {
    return;
  }
  UpdateMe();
}

void CBaseObject::Draw() {
  if (!IsActive()) {
    return;
  }
  DrawMe();
}

void CBaseObject::UpdateMe() {

}

void CBaseObject::DrawMe() {

}

#pragma endregion