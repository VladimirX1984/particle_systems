#include "StdAfx.h"

#include "Particle.h"

CParticle::CParticle(void) : CBaseObject() {
  m_iType = D_POINT;
  m_fLife = 0.0f;
  m_fFade = 0.005f;
  m_fMass = 1.0f;
  m_fSize = 1.0f;
  m_fAirResistance = 0.0f;
}

CParticle::~CParticle(void) {

}

void CParticle::Live() {
  m_fLife = 1.0f;
  SetActive(true);
}

void CParticle::Die() {
  SetActive(false);
}

void CParticle::SetType(int type) {
  m_iType = type;
}

void CParticle::SetFade(float fade) {
  m_fFade = fade;
}

void CParticle::SetMass(float mass) {
  m_fMass = mass;
}

void CParticle::SetSize(float size) {
  m_fSize = size;
}

void CParticle::SetAirResistance(float airResistance) {
  m_fAirResistance = airResistance;
}

void CParticle::SetColor(CColor &color) {
  m_oColor = color;
}

void CParticle::SetPosition(CVector &pos) {
  m_oPosition = pos;
}

void CParticle::SetDirection(CVector &direction) {
  m_oDirection = direction;
}

void CParticle::SetGravitation(CVector &gravitation) {
  m_oGravitation = gravitation;
}

//////////////////////////////////////////////////////////////////////////

void CParticle::UpdateMe() {
  if (m_fLife > 0.0f) {
    DecSize();
    Move();
    AirResist();
    Gravitate();

    m_fLife -= m_fFade;
  }
}

void CParticle::DecSize() {
  m_fSize *= 0.9f;
}

void CParticle::Move() {
  m_oPosition += (m_oDirection);
}

void CParticle::AirResist() {
  m_oDirection *= (1.0f - m_fAirResistance);
}


void CParticle::Gravitate() {
  CVector grav = m_oGravitation * m_fMass;
  m_oDirection -= grav;
}