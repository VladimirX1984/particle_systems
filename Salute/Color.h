#pragma once

class CColor : public CBaseObject {
 public:
  float r; // ������� ��������
  float g; // ������� ��������
  float b; // ����� ��������
  float a; // ������������

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
