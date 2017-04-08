#pragma once
#include "particlesystem.h"
#include <queue>

#include "FireworksVolleyBase.h"

class CFireworksVolleySimple : public CFireworksVolleyBase {
  public:
    CFireworksVolleySimple(void);
    virtual ~CFireworksVolleySimple(void);

  protected:
    virtual void SetParticleProperties(CParticle *particle) override;
};

//////////////////////////////////////////////////////////////////////////

class CExplodeParticleSystem : public CParticleSystem {
  public:
    CExplodeParticleSystem();
    virtual ~CExplodeParticleSystem();

  protected:
    virtual void SetParticleProperties(CParticle *particle) override;
};

class CFireworksVolleyExplode : public CFireworksVolleyBase {
  public:
    CFireworksVolleyExplode(void);
    virtual ~CFireworksVolleyExplode(void);

  protected:
    virtual void SetParticleProperties(CParticle *particle) override;
};

//////////////////////////////////////////////////////////////////////////

class CFireworksVolleyDouble : public CFireworksVolleyBase {
  public:
    CFireworksVolleyDouble(void);
    virtual ~CFireworksVolleyDouble(void);

  protected:
    virtual void SetParticleProperties(CParticle *particle) override;

  private:
    uint m_iCount;
};

