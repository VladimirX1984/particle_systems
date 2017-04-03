// Базовый класс для всех объектов
class CBaseObject {
 public:
  CBaseObject();
  virtual ~CBaseObject();

  __forceinline bool IsActive() { return m_bActive; };
  void SetActive(bool bActive);

  void Update();
  void Draw();

 protected:
  virtual void UpdateMe();
  virtual void DrawMe();

 private:
  bool m_bActive;
};