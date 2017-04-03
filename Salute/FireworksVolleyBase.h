#pragma once
#include "particlesystem.h"
#include <queue>

class CParticle;

// Базовый класс для залпов фейерверка
class CFireworksVolleyBase : public CParticleSystem {
 public:
  CFireworksVolleyBase(void);
  virtual ~CFireworksVolleyBase(void);

  // метод инициализации
  override bool Init(uint particleCount, uint subParticleCount,
                     std::string subParticleSystemClassName, std::string particleClassName);

  override void Live();

 protected:
  override void UpdateMe();

  override void DrawMe();

  override void SetRocketProperties();

 protected:
  // объект-ракета
  CParticle *m_oRocketParticle;
  // число подчастиц
  uint m_iSubParticleCount;

  typedef std::vector<CParticleSystem*> ParticleSystemVector;
  typedef std::vector<CParticleSystem*>::iterator ParticleSystemVectorIter;
  ParticleSystemVector m_particleSystemList;
  ParticleSystemVectorIter m_particleSystemIter;

  typedef std::queue<CParticleSystem*> ParticleSystemQueue;
  ParticleSystemQueue m_particleSystemQueue;
};

//////////////////////////////////////////////////////////////////////////

#define CREATE_FIREWORKS_VOLLEY(name) CREATE_OBJECT(name, CFireworksVolleyBase)
#define CREATE_PARTICLES(name) CREATE_OBJECT(name, CParticleSystem)