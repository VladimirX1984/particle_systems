#pragma once

class CColor : public CBaseObject {
 public:
  float r; // Красное значение
  float g; // Зеленое значение
  float b; // Синие значение
  float a; // Прозрачность

 public:
  CColor(void);
  CColor(float rr, float gg, float bb, float aa = 1.0f);
  CColor(const CColor &aColor);
  CColor(CColor *aColor);
  ~CColor(void);

  void SetColor(float rr, float gg, float bb, float aa = 1.0f);
  void SetColor(const CColor &aColor);
  void SetColor(CColor *aColor);
  CColor *Clone();

  inline operator float*() {
    float data[4] = {r, g, b, a};
    return data;
  };

};
