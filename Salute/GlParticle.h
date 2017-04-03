#pragma once

class CGlParticle : public CParticle {
 public:
  CGlParticle(void);
  virtual ~CGlParticle(void);

 protected:
  override void DrawMe();
};
