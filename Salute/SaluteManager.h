#pragma once

#include <queue>

class CFireworksVolleyBase;

// ���������, ����������� ��� ����� ������
struct TSaluteType {
  // ����� ������
  uint m_iParticleCount;
  // ����� ���-������, ���� ����, �� �� ����� ����������� ���������� ������, ����� ������� ������������� ������ m_sSubParticleSystemClassName
  uint m_iSubParticleCount;
  // ��� ������ ����� ������ (����������)
  std::string m_sFireworksVolleyClassName;
  // ��� ������ ������� ������, ������� ����������� �� ������ ����� ������ (����������)
  std::string m_sSubParticleSystemClassName;
  // ��� ������ �������
  std::string m_sParticleClassName;
};

// �����, ����������� �������
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
    // ������ ����������
    CVector m_oGravitation;

    typedef std::vector<CFireworksVolleyBase*> FireworksVolleyVector;
    typedef std::vector<CFireworksVolleyBase*>::iterator FireworksVolleyVectorIter;
    // ������ ��������: ������ ������ (����������)
    FireworksVolleyVector m_fireworksVolleyList;
    FireworksVolleyVectorIter fireworksVolleyIter;

    typedef std::queue<CFireworksVolleyBase*> FireworksVolleyQueue;
    // ������ ������ ������ (����������), ������� ���� ��������
    FireworksVolleyQueue m_fireworksVolleyQueue;

    uint m_iTimer;

    // ����� ������������� ������ ������ (����������)
    uint m_iVolleysCount;

    uint m_iInterval;
};
