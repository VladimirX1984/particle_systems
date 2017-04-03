
#include "StdAfx.h"
#include <math.h>
#include "Vector.h"

void CVector::RotateZ(float angle) {
  float cs = (float)cos(CMathEx::GradToRadians(angle));
  float sn = (float)sin(CMathEx::GradToRadians(angle));

  float xx = x * cs - y * sn;
  y = x * sn + y * cs;
  x = xx;
}

void CVector::RotateX(float angle) {
  float cs = (float)cos(CMathEx::GradToRadians(angle));
  float sn = (float)sin(CMathEx::GradToRadians(angle));

  float yy = y * cs - z * sn;
  z = y * sn + z * cs;
  y = yy;
}

void CVector::RotateY(float angle) {
  float cs = (float)cos(CMathEx::GradToRadians(-angle));
  float sn = (float)sin(CMathEx::GradToRadians(-angle));

  float xx = x * cs - z * sn;
  z = x * sn + z * cs;
  x = xx;
}

float CVector::GetAngleY(CVector vec) {
  float cs = x * vec.x + z * vec.z;
  cs /= sqrtf(x * x + z * z);
  cs /= sqrtf(vec.x * vec.x + vec.z * vec.z);
  return acosf(cs);
}

float CVector::GetAngleZ(CVector vec) {
  float cs = x * vec.x + z * vec.z;
  cs /= sqrtf(x * x + z * z);
  cs /= sqrtf(vec.x * vec.x + vec.z * vec.z);
  return acosf(cs);
}