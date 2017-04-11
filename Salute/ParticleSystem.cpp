#include "StdAfx.h"
#include "ParticleSystem.h"
#include "Particle.h"

CParticleSystem::CParticleSystem(void) : CBaseObject(),
  m_oPosition(CVector(0.0f, 0.0f, 0.0f)), m_bLive(false) {
}

CParticleSystem::~CParticleSystem(void) {
  auto it = m_particleList.begin(), itend = m_particleList.end();

  for (; it != itend; ++it) {
    CParticle *particle = *it;
    delete particle;
  }
  m_particleList.clear();
}

bool CParticleSystem::Init(uint particleCount, std::string particleClassName) {
  // Инициализация всех частиц
  for (uint i = 0; i < particleCount; ++i) {

    CParticle *particle = (CParticle*)CObjectClassManager::GetObjectClassManager().GetObject(
                            particleClassName, "CParticle");
    if (particle == NULL) {
      std::string sMsg = "Класс частицы '" + particleClassName +
                         "' не определен в системе.";
      MessageBox(NULL, sMsg.c_str(), "Ошибка", MB_OK | MB_ICONEXCLAMATION);
      return false;
    }
    m_particleList.push_back(particle);
  }
  return true;
}

void CParticleSystem::Live() {
  SetActive(true);
  m_bLive = true;
  auto it = m_particleList.begin(), itend = m_particleList.end();

  for (; it != itend; ++it) {
    CParticle *particle = *it;
    particle->Live();
  }
}

void CParticleSystem::SetColor(CColor &color) {
  m_oColor = color;
}

void CParticleSystem::SetGravitation(CVector &gravitation) {
  m_oGravitation = gravitation;
}

void CParticleSystem::SetPosition(CVector &position) {
  m_oPosition = position;
}

void CParticleSystem::Apply() {
  auto it = m_particleList.begin(), itend = m_particleList.end();

  for (; it != itend; ++it) {
    auto particle = *it;
    SetParticleProperties(particle);
  }
}

//////////////////////////////////////////////////////////////////////////

void CParticleSystem::UpdateMe() {
  if (m_bLive) {
    bool bLive = false;
    auto it = m_particleList.begin(), itend = m_particleList.end();
    for (; it != itend; ++it) {
      auto particle = *it;
      particle->Update();
      bLive |= particle->IsLive();
    }
    m_bLive = bLive;
    if (!m_bLive) {
      SetActive(false);
    }
  }
}

void CParticleSystem::DrawMe() {
  if (!m_bLive) {
    return;
  }

  auto it = m_particleList.begin(), itend = m_particleList.end();
  for (; it != itend; ++it) {
    auto particle = *it;
    particle->Draw();
  }
}

void CParticleSystem::SetParticleProperties(CParticle *particle) {
  particle->SetPosition(m_oPosition);
  particle->SetColor(m_oColor);
}

