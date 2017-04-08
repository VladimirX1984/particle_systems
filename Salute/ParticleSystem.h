#pragma once

#include <vector>
#include "Color.h"
#include "types.h"

class CParticle;

// ������� ������
class CParticleSystem : public CBaseObject {
  public:
    CParticleSystem(void);
    virtual ~CParticleSystem(void);

    // �����, ����������� ��������� ������� ������
    __forceinline bool IsLive() { return m_bLive;};

    virtual void Live();

    virtual void SetColor(CColor& color);
    void SetGravitation(CVector& gravitation);
    void SetPosition(CVector& position);

    // ����� �������������: count - ����� ������
    virtual bool Init(uint particleCount, std::string particleClassName);

    // ���������� ��-�� ��� ���� ������ �������
    virtual void Apply();

  protected:
    virtual void UpdateMe() override;

    virtual void DrawMe() override;

    // �����, ��������������� ��-�� ������� ����������
    virtual void SetParticleProperties(CParticle* particle);

  protected:
    typedef std::vector<CParticle*> ParticleVector;

    // ������ ������
    ParticleVector m_particleList;

    // ��������� �������
    bool m_bLive;

    // ���� ������
    CColor m_oColor;

    // ����������
    CVector m_oGravitation;

    // ��������� �������
    CVector m_oPosition;
};
