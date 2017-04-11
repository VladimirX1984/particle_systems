#pragma once

#include "Color.h"
#include "types.h"

// Частица
class CParticle : public CBaseObject {
  public:
    enum : unsigned int { D_POINT, D_POINT_RAND, D_LINE, D_TRIANGLE};

    CParticle(void);
    virtual ~CParticle(void);

    // установить св-во активности частицы и св-во жизнь в 1.0f
    void Live();

    // деактивировать частицу
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

    // уменьшить размер частицы
    virtual void DecSize();
    // движение частиц
    virtual void Move();
    // учет сопротивления воздуха
    virtual void AirResist();
    // учет гравитации
    virtual void Gravitate();
    // основной метод работы частицы

  protected:
    // Жизнь
    float m_fLife;

    // Тип частицы
    int m_iType;

    // Скорость угасания жизни
    float m_fFade;

    // Масса частицы
    float m_fMass;

    // Размер частицы
    float m_fSize;

    // Сопротивляемость воздуха
    float m_fAirResistance;

    // Цвет
    CColor m_oColor;

    // Позиция
    CVector m_oPosition;

    // Направление движения
    CVector m_oDirection;

    // Гравитация
    CVector m_oGravitation;
};

#define CREATE_PARTICLE(name) CREATE_OBJECT(name, CParticle)