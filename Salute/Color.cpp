#include "StdAfx.h"
#include "Color.h"

CColor::CColor(void) {
  r = g = b = 1.0f;
  a = 1.0f;
}

CColor::CColor(float rr, float gg, float bb, float aa /*= 0.0f*/) {
  SetColor(rr, gg, bb, aa);
}

CColor::CColor(const CColor& color) {
  SetColor(color);
}

CColor::CColor(CColor* color) {
  SetColor(color);
}

CColor::~CColor(void) {
}

void CColor::SetColor(float rr, float gg, float bb, float aa /*= 0.0f*/) {
  r = rr;
  g = gg;
  b = bb;
  a = aa;
}

void CColor::SetColor(const CColor& aColor) {
  r = aColor.r;
  g = aColor.g;
  b = aColor.b;
  a = aColor.a;
}

void CColor::SetColor(CColor* aColor) {
  SetColor(aColor->r, aColor->g, aColor->b, aColor->a);
}

CColor *CColor::Clone() {
  CColor *pColor = new CColor(r, g, b, a);
  return pColor;
}
