#pragma once

#include "Color.h"
#include "types.h"

// �������
class CParticle : public CBaseObject {
  public:
    enum : unsigned int { D_POINT, D_POINT_RAND, D_LINE, D_TRIANGLE};

    CParticle(void);
    virtual ~CParticle(void);

    // ���������� ��-�� ���������� ������� � ��-�� ����� � 1.0f
    void Live();

    // �������������� �������
    void Die();

    void SetType(int type);
    void SetFade(float fade);
    void SetMass(float mass);

    void SetSize(float size);
    void SetAirResistance(float airResistance);

    void SetColor(CColor &color);
    void SetPosition(CVector &pos);
    void SetDirection(CVector &direction);
    void SetGravitation(CVector &gravitation);

    __forceinline bool IsLive() { return m_fLife > 0.0f && IsActive(); };
    __forceinline CColor& GetColor() { return m_oColor; };
    __forceinline CVector& GetPosition() { return m_oPosition; };
    __forceinline CVector& GetDirection() { return m_oDirection; };
    __forceinline CVector& GetGravitation() { return m_oGravitation; };


  protected:
    virtual void UpdateMe() override;

    // ��������� ������ �������
    virtual void DecSize();
    // �������� ������
    virtual void Move();
    // ���� ������������� �������
    virtual void AirResist();
    // ���� ����������
    virtual void Gravitate();
    // �������� ����� ������ �������

  protected:
    // �����
    float m_fLife;

    // ��� �������
    int m_iType;

    // �������� �������� �����
    float m_fFade;

    // ����� �������
    float m_fMass;

    // ������ �������
    float m_fSize;

    // ���������������� �������
    float m_fAirResistance;

    // ����
    CColor m_oColor;

    // �������
    CVector m_oPosition;

    // ����������� ��������
    CVector m_oDirection;

    // ����������
    CVector m_oGravitation;
};

#define CREATE_PARTICLE(name) CREATE_OBJECT(name, CParticle)