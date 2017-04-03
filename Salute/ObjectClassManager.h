#pragma once

#include <string>
#include <vector>
#include "BaseObject.h"

// Базовый класс для создания объектов
class CBaseObjectCreator {
 public:
  CBaseObjectCreator() {};
  virtual ~CBaseObjectCreator() {};

 public:
  virtual CBaseObject* CreateInstance() const = 0;
};

// Класс для создания объектов типа className
template <class className> class CObjectCreater : public CBaseObjectCreator {
 public:
  className* CreateInstance() const {
    return new className;
  };
};

// Базовый класс для создания объектов типа className, у которых конструктор принимает один аргумент типа CBaseObject
class CBaseObjectCreatorByObj : public CBaseObjectCreator {
 public:
  CBaseObjectCreatorByObj() {};
  virtual ~CBaseObjectCreatorByObj() {};

 public:
  virtual CBaseObject* CreateInstance(CBaseObject* obj) const = 0;
};

// Класс для создания объектов типа className, у которых конструктор принимает один аргумент типа objClassName
template <class className, class objClassName> class CObjectCreaterByObj :
  public CBaseObjectCreatorByObj {
 public:
  className* CreateInstance() const {
    return NULL;
  };

  className* CreateInstance(CBaseObject* obj) const {
    return new className((objClassName*) obj);
  };
};

// Класс, хранящий
// 1. объект-создатель
// 2. имя класса
// 3. тип класса - родителя
class CObjectService {
 public:
  CObjectService(std::string name, std::string type, CBaseObjectCreator* objectCreator);

 public:
  BOOL compareName(std::string name, std::string type);
  CBaseObjectCreator* getObjectCreator();

 protected:
  std::string name;
  std::string type;
  CBaseObjectCreator* objectCreator;
};

class CObjectClassManager {
 public:
  CObjectClassManager();
  virtual ~CObjectClassManager();

 public:
  static CObjectClassManager& GetObjectClassManager();

  CObjectService* GetObjectService(std::string name, std::string type);

  CBaseObjectCreator* GetObjectCreator(std::string name, std::string type);

  CBaseObject* GetObject(std::string name, std::string type);

  CBaseObject* GetObject(std::string name, std::string type, CBaseObject* obj);

  void RegisterService(std::string name, std::string serviceType, CBaseObjectCreator* objectCreator);

  void UnRegisterAllServices();

 protected:
  std::vector<CObjectService*> objectClassList;
};

#define _CREATE_OBJECT(name, _className, descriptor) \
class CObject_##_className##_##name##_Registration { \
public: \
  CObject_##_className##_##name##_Registration(CObjectClassManager*  objectClassMgr) \
{ \
  objectClassMgr->RegisterService(#name, #_className, &descriptor); \
} \
  int _warning; \
}; \
  CObject_##_className##_##name##_Registration \
  CObject_##_className##_##name##_Registration_Instance(&CObjectClassManager::GetObjectClassManager());

#define CREATE_OBJECT(name, _className) \
  static CObjectCreater<name> _className_##name##_##descriptor; \
  _CREATE_OBJECT(name, _className, _className_##name##_##descriptor)

#define CREATE_OBJECT_BY_OBJECT(name, _className, objClassName) \
  static CObjectCreater2<name, objClassName> _className_objClassName_##name##_##descriptor; \
  _CREATE_OBJECT(name, _className, _className_objClassName_##name##_##descriptor)
