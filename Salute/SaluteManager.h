#pragma once

#include <queue>

class CFireworksVolleyBase;

// Структура, описывающая тип залпа салюта
struct TSaluteType {
  // число частиц
  uint m_iParticleCount;
  // число суб-частиц, если ноль, то не будет создаваться подсистема частиц, класс которые соответствует строке m_sSubParticleSystemClassName
  uint m_iSubParticleCount;
  // имя класса залпа салюта (фейерверка)
  std::string m_sFireworksVolleyClassName;
  // имя класса системы частиц, которая запускается от частиц залпа салюта (фейерверка)
  std::string m_sSubParticleSystemClassName;
  // имя класса частицы
  std::string m_sParticleClassName;
};

// Класс, управляющий салютом
class CSaluteManager : public CBaseObject {
  public:
    CSaluteManager(void);
    ~CSaluteManager(void);

    void AddFireworks(uint particleCount, uint subParticleCount, std::string className,
                      std::string subParicleSystemClassName, std::string particleClassName);
    bool Init(uint volleysCount, uint particlesCount, uint interval);

  protected:
    virtual void UpdateMe();
    virtual void DrawMe();

  private:
    typedef std::vector<TSaluteType*> SaluteTypesVector;
    typedef std::vector<TSaluteType*>::iterator SaluteTypesVectorIter;
    SaluteTypesVector m_saluteTypesList;
    SaluteTypesVectorIter saluteTypesIter;

  private:
    // вектор гравитации
    CVector m_oGravitation;

    typedef std::vector<CFireworksVolleyBase*> FireworksVolleyVector;
    typedef std::vector<CFireworksVolleyBase*>::iterator FireworksVolleyVectorIter;
    // список объектов: залпов салюта (фейерверка)
    FireworksVolleyVector m_fireworksVolleyList;
    FireworksVolleyVectorIter fireworksVolleyIter;

    typedef std::queue<CFireworksVolleyBase*> FireworksVolleyQueue;
    // список залпов салюта (фейерверка), которые были запущены
    FireworksVolleyQueue m_fireworksVolleyQueue;

    uint m_iTimer;

    // число одновременных залпов салюта (фейерверка)
    uint m_iVolleysCount;

    uint m_iInterval;
};
