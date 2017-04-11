#include "StdAfx.h"
#include "FireworksVolleyBase.h"
#include "Particle.h"

CFireworksVolleyBase::CFireworksVolleyBase(void) {
  m_oRocketParticle = NULL;
  m_iSubParticleCount = 0;
}

CFireworksVolleyBase::~CFireworksVolleyBase(void) {
  if (m_oRocketParticle != NULL) {
    delete m_oRocketParticle;
  }

  while (!m_particleSystemQueue.empty()) {
    m_particleSystemQueue.pop();
  }

  ParticleSystemVectorIter it = m_particleSystemList.begin(),
                           itend = m_particleSystemList.end();

  for (; it != itend; ++it) {
    CParticleSystem *particleSystem = *it;
    delete particleSystem;
  }
  m_particleSystemList.clear();
}

bool CFireworksVolleyBase::Init(uint particleCount,
                                uint subParticleCount, std::string subParticleSystemClassName,
                                std::string particleClassName) {
  if (!CParticleSystem::Init(particleCount, particleClassName)) {
    return false;
  }
  m_iSubParticleCount = subParticleCount;
  m_oRocketParticle = (CParticle*)CObjectClassManager::GetObjectClassManager().GetObject(
                        particleClassName, "CParticle");
  if (m_oRocketParticle == NULL) {
    std::string sMsg = "Класс частицы залпа '" + particleClassName +
                       "' не определен в системе.";
    MessageBox(NULL, sMsg.c_str(), "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }
  SetRocketProperties();
  if (m_iSubParticleCount > 0) {
    // Инициализация всех частиц
    for (uint i = 0; i < particleCount; ++i) {
      CParticleSystem *subParticleSystem = (CFireworksVolleyBase*)
                                           CObjectClassManager::GetObjectClassManager().GetObject(subParticleSystemClassName,
                                               "CParticleSystem");
      if (!subParticleSystem) {
        std::string sMsg = "Класс подсистемы частиц '" + subParticleSystemClassName +
                           "' не определен в системе.";
        MessageBox(NULL, sMsg.c_str(), "Ошибка", MB_OK | MB_ICONEXCLAMATION);
        return false;
      }
      if (!subParticleSystem->Init(m_iSubParticleCount, particleClassName)) {
        return false;
      }
      m_particleSystemList.push_back(subParticleSystem);
    }
    m_particleSystemIter = m_particleSystemList.begin();
  }
  return true;
}

void CFireworksVolleyBase::Live() {
  CParticleSystem::Live();
  m_oRocketParticle->Live();
  m_oRocketParticle->SetPosition(CVector(0.0f, 0.0f, 0.0f));
  SetRocketProperties();
}

//////////////////////////////////////////////////////////////////////////

void CFireworksVolleyBase::UpdateMe() {
  if (!m_bLive) {
    while (!m_particleSystemQueue.empty()) {
      m_particleSystemQueue.pop();
    }
    return;
  }

  if (m_oRocketParticle->IsLive()) {
    m_oRocketParticle->Update();
    return;
  }

  if (m_oRocketParticle->IsActive()) {
    m_oRocketParticle->Die();
    auto it = m_particleList.begin(), itend = m_particleList.end();
    for (; it != itend; ++it) {
      auto particle = *it;
      particle->SetPosition(m_oRocketParticle->GetPosition());
    }
  }

  bool bLive = false;
  auto it = m_particleList.begin(), itend = m_particleList.end();
  if (m_iSubParticleCount == 0) {
    for (; it != itend; ++it) {
      auto particle = *it;
      particle->Update();
      bLive |= particle->IsLive();
    }
    m_bLive = bLive;
    if (!m_bLive) {
      SetActive(false);
    }
    return;
  }

  bool bActive_ = true;
  if (m_bLive && !m_particleSystemQueue.empty()) {
    bool bLive = false;
    for each (CParticleSystem * particleSystem in m_particleSystemQueue._Get_container()) {
      particleSystem->Update();
      bLive |= particleSystem->IsLive();
    }
    m_bLive = bLive;
    if (!m_bLive) {
      bActive_ = false;
    }
  }

  for (; it != itend; ++it) {
    auto particle = *it;
    if (!particle->IsActive()) {
      continue;
    }
    if (!bActive_) {
      bActive_ = true;
      m_bLive = true;
    }

    if (particle->IsLive()) {
      particle->Update();
    }
    else {
      particle->Die();

      auto particleSystem = *m_particleSystemIter;
      particleSystem->Live();
      particleSystem->SetPosition(particle->GetPosition());
      particleSystem->SetColor(m_oColor);
      particleSystem->SetGravitation(m_oGravitation);
      particleSystem->Apply();
      m_particleSystemQueue.push(particleSystem);

      if (++m_particleSystemIter == m_particleSystemList.end()) {
        m_particleSystemIter = m_particleSystemList.begin();
      }
    }
  }

  if (!bActive_) {
    SetActive(false);
  }
}

void CFireworksVolleyBase::DrawMe() {
  if (!m_bLive) {
    return;
  }

  if (m_oRocketParticle->IsLive()) {
    m_oRocketParticle->Draw();
    return;
  }
  bool bLive = false;
  auto it = m_particleList.begin(), itend = m_particleList.end();
  for (; it != itend; ++it) {
    auto particle = *it;
    particle->Draw();
  }

  if (m_iSubParticleCount == 0) {
    return;
  }

  if (m_bLive && !m_particleSystemQueue.empty()) {
    for each (auto particleSystem in m_particleSystemQueue._Get_container()) {
      particleSystem->Draw();
    }
  }
}

void CFireworksVolleyBase::SetRocketProperties() {
  m_oRocketParticle->SetType(CParticle::D_LINE);
  CParticleSystem::SetParticleProperties(m_oRocketParticle);

  m_oRocketParticle->SetSize(1.5f);
  m_oRocketParticle->SetAirResistance(0.0f);
  m_oRocketParticle->SetFade(0.07f - (float)(rand() % 100) / 8000.0f);
  m_oRocketParticle->SetMass(5.0f);

  CVector vDirection = CVector(1.5f - 3.0f * CMathEx::Randf(), 4.5f + 2.5f * CMathEx::Randf(),
                               1.5f - 3.0f * CMathEx::Randf());

  m_oRocketParticle->SetDirection(vDirection);
  m_oRocketParticle->SetGravitation(m_oGravitation);
}