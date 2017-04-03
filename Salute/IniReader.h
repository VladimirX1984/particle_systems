#pragma once

class CIniReader {
 public:
  CIniReader(void);
  ~CIniReader(void);

  bool GetFullscreen();
  uint GetWidth();
  uint GetHeight();
  uint GetVolleysCount();
  uint GetParticlesCount();
  uint GetSubParticlesCount();
  uint GetInterval();

 private:
  std::string m_sFileName;
};
