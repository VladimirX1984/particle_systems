#pragma once
class CMathEx {
  public:
    static float GradToRadians(float grad) { return (grad / 180.0f * 3.14159265358f); }

    static float Min(float a, float b) { return a > b ? b : a; }
    static float Min(float a, float b, float c) { return Min(Min(a, b), c); }
    static float Min(float a, float b, float c, float d) { return Min(Min(a, b), Min(c, d)); }

    // Random 0.0f to 1.0f
    static inline float Randf() { return ((float)rand()) / ((float)RAND_MAX); }
};

