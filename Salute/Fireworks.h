#pragma once
#include "particlesystem.h"
#include <queue>

#include "FireworksVolleyBase.h"

class CFireworksVolleySimple : public CFireworksVolleyBase {
 public:
  CFireworksVolleySimple(void);
  virtual ~CFireworksVolleySimple(void);

 protected:
  override void SetParticleProperties(CParticle *particle);
};

//////////////////////////////////////////////////////////////////////////

class CExplodeParticleSystem : public CParticleSystem {
 public:
  CExplodeParticleSystem();
  virtual ~CExplodeParticleSystem();

 protected:
  override void SetParticleProperties(CParticle *particle);
};

class CFireworksVolleyExplode : public CFireworksVolleyBase {
 public:
  CFireworksVolleyExplode(void);
  virtual ~CFireworksVolleyExplode(void);

 protected:
  override void SetParticleProperties(CParticle *particle);
};

//////////////////////////////////////////////////////////////////////////

class CFireworksVolleyDouble : public CFireworksVolleyBase {
 public:
  CFireworksVolleyDouble(void);
  virtual ~CFireworksVolleyDouble(void);

 protected:
  override void SetParticleProperties(CParticle *particle);

 private:
  uint m_iCount;
};

