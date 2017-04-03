#include "StdAfx.h"
#include "IniReader.h"

CIniReader::CIniReader(void) {
  char szModuleName[MAX_PATH];
  ZeroMemory(szModuleName, sizeof(szModuleName));
  if (GetModuleFileName(0, szModuleName, _countof(szModuleName) - 2) <= 0) {
    lstrcpy(szModuleName, _T("Unknown"));
  }

  std::string sModuleName = szModuleName;
  int pos = sModuleName.find_last_of(".");
  if (pos > 0) {
    m_sFileName = sModuleName.substr(0, pos) + ".ini";
  }
  else {
    m_sFileName = "salute.ini";
  }
}

CIniReader::~CIniReader(void) {
}

bool CIniReader::GetFullscreen() {
  int aRes = GetPrivateProfileInt("settings", "fullscreen", 0, m_sFileName.c_str());
  return aRes == 1;
}

uint CIniReader::GetWidth() {
  int aRes = GetPrivateProfileInt("settings", "width", 800, m_sFileName.c_str());
  return (uint) aRes;
}

uint CIniReader::GetHeight() {
  int aRes = GetPrivateProfileInt("settings", "height", 600, m_sFileName.c_str());
  return (uint) aRes;
}

uint CIniReader::GetVolleysCount() {
  int aRes = GetPrivateProfileInt("settings", "volleys_count", 1, m_sFileName.c_str());
  return (uint) aRes;
}

uint CIniReader::GetParticlesCount() {
  int aRes = GetPrivateProfileInt("settings", "particles_count", 1000, m_sFileName.c_str());
  return (uint) aRes;
}

uint CIniReader::GetSubParticlesCount() {
  int aRes = GetPrivateProfileInt("settings", "sub_particles_count", 20, m_sFileName.c_str());
  return (uint) aRes;
}

uint CIniReader::GetInterval() {
  int aRes = GetPrivateProfileInt("settings", "interval", 2000, m_sFileName.c_str());
  return (uint) aRes;
}