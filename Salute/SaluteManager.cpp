#include "StdAfx.h"
#include "SaluteManager.h"
#include "FireworksVolleyBase.h"

#define COLOR_COUNT 31

static float colors[COLOR_COUNT][3] = { // Цветовая радуга
  {1.0f, 0.5f, 0.5f}, {1.0f, 0.75f, 0.5f}, {1.0f, 1.0f, 0.5f}, {1.0f, 1.0f, 0.75f}, {0.75f, 1.0f, 0.75f}, {0.75f, 1.0f, 0.5f},
  {0.5f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.75f}, {0.5f, 1.0f, 1.0f}, {0.75f, 1.0f, 1.0f}, {0.75f, 0.75f, 1.0f}, {0.5f, 0.75f, 1.0f},
  {0.5f, 0.5f, 1.0f}, {0.75f, 0.5f, 1.0f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.75f, 1.0f}, {1.0f, 0.75f, 0.75f}, {1.0f, 0.5f, 0.75f},
  {1.0f, 0.5f, 0.25f}, {1.0f, 0.25f, 0.5f}, {1.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.5f}, {1.0f, 0.25f, 0.0f}, {1.0f, 0.0f, 0.25f}, {1.0f, 0.0f, 0.0f},
  {0.0f, 0.5f, 1.0f}, {0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.0f}
};

CSaluteManager::CSaluteManager(void) {
  m_oGravitation = CVector(0.0f, 0.01f, 0.0f);
  m_iTimer = 0;
  m_iVolleysCount = 1;
}

CSaluteManager::~CSaluteManager(void) {
  while (!m_fireworksVolleyQueue.empty()) {
    m_fireworksVolleyQueue.pop();
  }

  auto it = m_fireworksVolleyList.begin(), itend = m_fireworksVolleyList.end();
  for (; it != itend; ++it) {
    CFireworksVolleyBase *fireworksVolley = *it;
    delete fireworksVolley;
  }
  m_fireworksVolleyList.clear();
}

void CSaluteManager::AddFireworks(uint particleCount, uint subParticleCount,
                                  std::string fireworksVolleyClassName, std::string subParicleSystemClassName,
                                  std::string particleClassName) {
  TSaluteType *pSaluteType = new TSaluteType();
  pSaluteType->m_iParticleCount = particleCount;
  pSaluteType->m_iSubParticleCount = subParticleCount;
  pSaluteType->m_sFireworksVolleyClassName = fireworksVolleyClassName;
  pSaluteType->m_sSubParticleSystemClassName = subParicleSystemClassName;
  pSaluteType->m_sParticleClassName = particleClassName;
  m_saluteTypesList.push_back(pSaluteType);
}

bool CSaluteManager::Init(uint volleysCount, uint particlesCount, uint interval) {
  if (volleysCount < 1) {
    MessageBox(NULL, "Количество залпов должно быть не менее 1.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  if (particlesCount < 10) {
    MessageBox(NULL, "Количество частиц должно быть не менее 10.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  if (volleysCount > 5) {
    MessageBox(NULL, "Количество залпов должно быть не более 5.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  if (particlesCount * volleysCount > 100000) {
    MessageBox(NULL,
               "Произведение количества залпов и количества частиц в залпе должно быть не более 100000.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  if (m_saluteTypesList.size() == 0) {
    MessageBox(NULL,
               "Не добавлены типы залпов салюта. \nДобавьте хотя бы один тип залпа, используя метод 'AddFireworks'.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  if (interval < 1000 || interval > 30000) {
    MessageBox(NULL,
               "Интервал между залпами должен быть не менее 1 секунды и не более 30 секунд.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  m_iVolleysCount = volleysCount;
  m_iInterval = interval;

  for (uint j = 0; j < m_iVolleysCount; ++j) {
    auto it = m_saluteTypesList.begin(), itend = m_saluteTypesList.end();
    for (; it != itend; ++it) {
      TSaluteType *pSaluteType = *it;
      CFireworksVolleyBase *fireworksVolley = (CFireworksVolleyBase*)
                                              CObjectClassManager::GetObjectClassManager().GetObject(
                                                pSaluteType->m_sFireworksVolleyClassName, "CFireworksVolleyBase");
      if (!fireworksVolley) {
        std::string sMsg = "Класс залпа салюта '" + pSaluteType->m_sFireworksVolleyClassName
                           + "' не определен в системе.";
        MessageBox(NULL, sMsg.c_str(), "Ошибка", MB_OK | MB_ICONEXCLAMATION);
        return false;
      }

      if (pSaluteType->m_iParticleCount * volleysCount > 100000) {
        MessageBox(NULL,
                   "Произведение количества залпов и количества всех частиц в залпе должно быть не более 100000.",
                   "Ошибка", MB_OK | MB_ICONEXCLAMATION);
        return false;
      }

      int particlesCount_ = pSaluteType->m_iSubParticleCount > 0 ?
                            pSaluteType->m_iParticleCount / pSaluteType->m_iSubParticleCount : pSaluteType->m_iParticleCount;
      if (particlesCount_ < 1) {
        particlesCount_ = 1;
      }

      if (!fireworksVolley->Init(particlesCount_, pSaluteType->m_iSubParticleCount,
                                 pSaluteType->m_sSubParticleSystemClassName, pSaluteType->m_sParticleClassName)) {
        return false;
      }
      m_fireworksVolleyList.push_back(fireworksVolley);
    }
  }

  fireworksVolleyIter = m_fireworksVolleyList.begin();

  return true;
}

void CSaluteManager::UpdateMe() {
  for each (auto fireworksVolley in m_fireworksVolleyQueue._Get_container()) {
    fireworksVolley->Update();
  }

  if (m_fireworksVolleyQueue.size() > m_fireworksVolleyList.size()) {
    auto fireworksVolley = m_fireworksVolleyQueue.front();
    if (!fireworksVolley->IsLive()) {
      m_fireworksVolleyQueue.pop();
    }
  }

  if (GetTickCount() - m_iTimer >= m_iInterval) {
    bool bTimerChange = false;
    for (uint i = 0; i < m_iVolleysCount; ++i) {
      CFireworksVolleyBase *fireworksVolley = *fireworksVolleyIter;

      if (!fireworksVolley->IsLive()) {
        fireworksVolley->Live();
        CColor color = new CColor(colors[rand() % COLOR_COUNT][0], colors[rand() % COLOR_COUNT][1],
                                  colors[rand() % COLOR_COUNT][2]);
        fireworksVolley->SetColor(color);
        fireworksVolley->SetGravitation(m_oGravitation);
        fireworksVolley->Apply();
        m_fireworksVolleyQueue.push(fireworksVolley);
        bTimerChange = true;
      }
      if (++fireworksVolleyIter == m_fireworksVolleyList.end()) {
        fireworksVolleyIter = m_fireworksVolleyList.begin();
      }
    }
    if (bTimerChange) {
      m_iTimer = GetTickCount();
    }
  }
}

void CSaluteManager::DrawMe() {
  for each (CFireworksVolleyBase * fireworksVolley in m_fireworksVolleyQueue._Get_container()) {
    fireworksVolley->Draw();
  }
}