
#pragma once

#include <math.h>

class CVector {
  public:
    float x, y, z;

  public:
    inline CVector(float xx, float yy, float zz = 0.0f) {
      x = xx;
      y = yy;
      z = zz;
    }

    inline CVector() {
      x = y = z = 0;
    }

    inline CVector Cross(CVector b) {
      return (CVector(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x));
    }

    inline void Cross(const CVector& a, const CVector& b) {
      x = a.y * b.z - a.z * b.y;
      y = a.z * b.x - a.x * b.z;
      z = a.x * b.y - a.y * b.x;
    }

    inline void Set(float dx, float dy, float dz) {
      x = dx;
      y = dy;
      z = dz;
    }

    inline void Set(const CVector& p) {
      x = p.x;
      y = p.y;
      z = p.z;
    }

    inline void Normalize() {
      float r = sqrtf(x * x + y * y + z * z);

      if (r != 0.0f) {
        x = x / r;
        y = y / r;
        z = z / r;
      }
    }

    inline float Dot(const CVector& b) {
      return x * b.x + y * b.y + z * b.z;
    }

    inline static float Dot(const CVector& a, const CVector& b) {
      return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline CVector operator -(CVector a) {
      CVector b;

      b.x = x - a.x;
      b.y = y - a.y;
      b.z = z - a.z;

      return b;
    }

    inline CVector operator +(CVector a) {
      CVector b;

      b.x = x + a.x;
      b.y = y + a.y;
      b.z = z + a.z;

      return b;
    }

    inline void operator -=(CVector a) {
      x -= a.x;
      y -= a.y;
      z -= a.z;
    }

    inline void operator +=(CVector a) {
      x += a.x;
      y += a.y;
      z += a.z;
    }

    inline void operator *=(float a) {
      x *= a;
      y *= a;
      z *= a;
    }

    inline void operator /= (float a) {
      x /= a;
      y /= a;
      z /= a;
    }

    inline CVector operator *(float a) {
      CVector tmp;

      tmp.x = x * a;
      tmp.y = y * a;
      tmp.z = z * a;

      return tmp;
    }

    inline CVector operator /(float a) {
      CVector tmp;

      tmp.x = x / a;
      tmp.y = y / a;
      tmp.z = z / a;

      return tmp;
    }

    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);

    float GetAngleY(CVector vec);
    float GetAngleZ(CVector vec);

    __forceinline float GetLength() {
      return sqrtf(x * x + y * y + z * z);
    }

    __forceinline float GetLengthSq() {
      return x * x + y * y + z * z;
    }

    CVector& operator =(const CVector& v) {
      x = v.x;
      y = v.y;
      z = v.z;

      return *this;
    }

    inline friend CVector operator %(const CVector& v, const CVector& u);

    inline CVector GetNormal(CVector& second) {
      CVector ret;
      ret.x = y * second.z - z * second.y;
      ret.y = z * second.x - x * second.z;
      ret.z = x * second.y - y * second.x;
      ret.Normalize();

      return ret;
    }

    inline float GetAngle(CVector& second) {
      if ((GetLength() == 0.0f) || (second.GetLength() == 0.0f)) {
        return 0.0f;
      }

      float arg = (x * second.x + y * second.y + z * second.z) /
                  (GetLength() * second.GetLength());
      if (arg >= 1) {
        return 0.0f;
      }

      if (arg <= -1) {
        return 3.141592f;
      }

      return acosf(arg);
    }

    __forceinline bool operator==(const CVector& o) const {
      return (x == o.x && y == o.y && z == o.z);
    }

    __forceinline bool operator!=(const CVector& o) const {
      return ((x != o.x) || (y != o.y) || (z != o.z));
    }

    inline operator float*() {
      float data[3] = {x, y, z};
      return data;
    };

    inline ~CVector() {
    }
};

CVector operator %(const CVector& v, const CVector& u) {
  return CVector(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z,
                 v.x * u.y - v.y * u.x);
}