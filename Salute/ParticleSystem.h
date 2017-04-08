#pragma once

#include <vector>
#include "Color.h"
#include "types.h"

class CParticle;

// Система частиц
class CParticleSystem : public CBaseObject {
  public:
    CParticleSystem(void);
    virtual ~CParticleSystem(void);

    // метод, возращающий состояние системы частиц
    __forceinline bool IsLive() { return m_bLive;};

    virtual void Live();

    virtual void SetColor(CColor& color);
    void SetGravitation(CVector& gravitation);
    void SetPosition(CVector& position);

    // метод инициализации: count - число частиц
    virtual bool Init(uint particleCount, std::string particleClassName);

    // установить св-ва для всех частиц системы
    virtual void Apply();

  protected:
    virtual void UpdateMe() override;

    virtual void DrawMe() override;

    // метод, устанавливающий св-ва частицы фейерверка
    virtual void SetParticleProperties(CParticle* particle);

  protected:
    typedef std::vector<CParticle*> ParticleVector;

    // список частиц
    ParticleVector m_particleList;

    // состояние системы
    bool m_bLive;

    // цвет частиц
    CColor m_oColor;

    // гравитация
    CVector m_oGravitation;

    // начальная позиция
    CVector m_oPosition;
};
