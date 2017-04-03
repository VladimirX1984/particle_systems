#include "StdAfx.h"
#include "Fireworks.h"
#include "Particle.h"

#pragma region CFireworksVolleySimple

CREATE_FIREWORKS_VOLLEY(CFireworksVolleySimple);

CFireworksVolleySimple::CFireworksVolleySimple(void) {
}

CFireworksVolleySimple::~CFireworksVolleySimple(void) {

}

//////////////////////////////////////////////////////////////////////////

void CFireworksVolleySimple::SetParticleProperties(CParticle *particle) {
  particle->SetType(CParticle::D_LINE);
  CParticleSystem::SetParticleProperties(particle);

  particle->SetSize(0.4f);
  particle->SetAirResistance(0.13f);
  particle->SetFade(0.05f - (float) (rand() % 100) / 4000.0f);
  particle->SetMass(4.0f);

  float xa = 360.0f * CMathEx::Randf();
  float ya = 180.0f * CMathEx::Randf();
  float sp = 3.0f + 2.0f * CMathEx::Randf();

  float sx = sp * sin(ya) * sin(xa);
  float sy = sp * cos(ya);
  float sz = sp * sin(ya) * cos(xa);

  CVector vDirection = CVector(sx, sy, sz);

  particle->SetDirection(vDirection);
  particle->SetGravitation(m_oGravitation);
}

#pragma endregion

#pragma region CExplodeParticleSystem

CREATE_PARTICLES(CExplodeParticleSystem);

CExplodeParticleSystem::CExplodeParticleSystem(void) {

}

CExplodeParticleSystem::~CExplodeParticleSystem(void) {

}

void CExplodeParticleSystem::SetParticleProperties(CParticle *particle) {
  particle->SetType(CParticle::D_POINT);
  CParticleSystem::SetParticleProperties(particle);

  particle->SetSize(0.2f);
  particle->SetAirResistance(0.13f);
  particle->SetFade(0.04f - (float) (rand() % 100) / 8000.0f);
  particle->SetMass(0.05f);

  float xa = 360.0f * CMathEx::Randf();
  float ya = 180.0f * CMathEx::Randf();
  float sp = 0.4f + 0.8f * CMathEx::Randf ();

  float sx = sp * sin(ya) * sin(xa);
  float sy = sp * cos(ya);
  float sz = sp * sin(ya) * cos(xa);

  particle->SetDirection(CVector(sx, sy, sz));
  particle->SetGravitation(m_oGravitation);
}

#pragma endregion

#pragma region CFireworksVolleyExplode

CREATE_FIREWORKS_VOLLEY(CFireworksVolleyExplode);

CFireworksVolleyExplode::CFireworksVolleyExplode(void) {
}

CFireworksVolleyExplode::~CFireworksVolleyExplode(void) {

}

//////////////////////////////////////////////////////////////////////////

void CFireworksVolleyExplode::SetParticleProperties(CParticle *particle) {
  particle->SetType(CParticle::D_POINT_RAND);
  CParticleSystem::SetParticleProperties(particle);

  particle->SetSize(0.45f);
  particle->SetAirResistance(0.13f);
  particle->SetFade(0.075f - (float) (rand() % 100) / 4000.0f);
  particle->SetMass(0.0f);

  float xa = 360.0f * CMathEx::Randf();
  float ya = 180.0f * CMathEx::Randf();
  float sp = 4.0f + 1.0f * CMathEx::Randf ();

  float sx = sp * sin(ya) * sin(xa);
  float sy = sp * cos(ya);
  float sz = sp * sin(ya) * cos(xa);

  particle->SetDirection(CVector(sx, sy, sz));
  particle->SetGravitation(m_oGravitation);
}

#pragma endregion

#pragma region CFireworksVolleyDouble

CREATE_FIREWORKS_VOLLEY(CFireworksVolleyDouble);

CFireworksVolleyDouble::CFireworksVolleyDouble(void) {
  m_iCount = 0;
}

CFireworksVolleyDouble::~CFireworksVolleyDouble(void) {

}

//////////////////////////////////////////////////////////////////////////

void CFireworksVolleyDouble::SetParticleProperties(CParticle *particle) {
  if (m_iCount % 7 < 5) {
    particle->SetType(CParticle::D_LINE);
    CParticleSystem::SetParticleProperties(particle);

    particle->SetSize(0.1f);
    particle->SetAirResistance(0.13f);
    particle->SetFade(0.05f - (float) (rand() % 100) / 8000.0f);
    particle->SetMass(0.04f);

    float xa = 360.0f * CMathEx::Randf();
    float ya = 180.0f * CMathEx::Randf();
    float sp = 1.0f + 3.0f * CMathEx::Randf();

    float sx = sp * sin(ya) * sin(xa);
    float sy = sp * cos(ya);
    float sz = sp * sin(ya) * cos(xa);

    particle->SetDirection(CVector(sx, sy, sz));
    particle->SetGravitation(m_oGravitation);
  }
  else {
    particle->SetType(CParticle::D_LINE);
    CParticleSystem::SetParticleProperties(particle);

    particle->SetSize(0.2f);
    particle->SetAirResistance(0.13f);
    particle->SetFade(0.035f - (float) (rand() % 100) / 5000.0f);
    particle->SetMass(0.04f);

    float xa = 360.0f * CMathEx::Randf();
    float ya = 180.0f * CMathEx::Randf();
    float sp = 4.0f + 1.0f * CMathEx::Randf();

    float sx = sp * sin(ya) * sin(xa);
    float sy = sp * cos(ya);
    float sz = sp * sin(ya) * cos(xa);

    particle->SetDirection(CVector(sx, sy, sz));
    particle->SetGravitation(m_oGravitation);
  }
}

#pragma endregion
