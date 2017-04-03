#include "StdAfx.h"
#include "ObjectClassManager.h"

#pragma region CObjectService

CObjectService::CObjectService(std::string name, std::string type,
                               CBaseObjectCreator* objectCreator) {
  this->name = name;
  this->type = type;
  this->objectCreator = objectCreator;
}

BOOL CObjectService::compareName(std::string name, std::string type) {
  return (this->name.compare(name) == 0 && this->type.compare(type) == 0);
}

CBaseObjectCreator* CObjectService::getObjectCreator() {
  return objectCreator;
}

#pragma endregion

#pragma region CObjectClassManager

CObjectClassManager::CObjectClassManager() {
}

CObjectClassManager::~CObjectClassManager() {
  UnRegisterAllServices();
}

CObjectClassManager& CObjectClassManager::GetObjectClassManager() {
  static CObjectClassManager objectClassManager;
  return objectClassManager;
}

CObjectService* CObjectClassManager::GetObjectService(std::string name, std::string type) {
  auto it = objectClassList.begin(), end = objectClassList.end();
  for (; it != end; ++it) {
    auto objService = *it;
    if (objService->compareName(name, type)) {
      return objService;
    }
  }
  return NULL;
}

CBaseObjectCreator* CObjectClassManager::GetObjectCreator(std::string name, std::string type) {
  auto objectService = GetObjectService(name, type);
  if (objectService != NULL) {
    return objectService->getObjectCreator();
  }
  return NULL;
}

CBaseObject* CObjectClassManager::GetObject(std::string name, std::string type) {
  CBaseObjectCreator* objectCreater = GetObjectCreator(name, type);
  if (objectCreater != NULL) {
    return objectCreater->CreateInstance();
  }
  return NULL;
}

CBaseObject* CObjectClassManager::GetObject(std::string name, std::string type, CBaseObject* obj) {
  CBaseObjectCreatorByObj* objectCreater = (CBaseObjectCreatorByObj*)GetObjectCreator(name, type);
  if (objectCreater != NULL) {
    return objectCreater->CreateInstance(obj);
  }
  return NULL;
}

void CObjectClassManager::RegisterService(std::string name, std::string type,
                                          CBaseObjectCreator* objectCreator) {
  if (GetObjectService(name, type) == NULL) {
    objectClassList.push_back(new CObjectService(name, type, objectCreator));
  }
}

void CObjectClassManager::UnRegisterAllServices() {
  objectClassList.clear();
}

#pragma endregion