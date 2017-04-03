
#include "stdafx.h"

#include <gl\gl.h>      // Заголовочный файл библиотеки OpenGL32
#include <gl\glu.h>     // Заголовочный файл библиотеки GLu32

#include "Salute.h"
#include "SaluteManager.h"
#include "IniReader.h"
//#include "GlParticle.h"

HDC     hDC = NULL;  // Частный контекст GDI устройства
HGLRC   hRC = NULL; // Постоянный контекст отображения
HWND    hWnd = NULL;
HINSTANCE hInstance;
UINT_PTR  hTimer = 0;

BOOL done = FALSE; // Булевская переменная выхода из цикла
bool keys[256];    // Массив применяемый для подпрограммы клавиатуры
bool active = true; // Флаг "Активное" окна. По умолчанию true.
bool fullscreen = false;

const char* wndName = "SaluteWnd";

CSaluteManager saluteManager; // Менеджер салюта
CIniReader iniReader; // Читатель параметров из ini-файлов

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int InitGL(GLvoid) {
  glShadeModel(GL_SMOOTH); // Разрешить плавное затенение
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return TRUE;
}


GLvoid ResizeGLScene(GLsizei width, GLsizei height) {
  if (height == 0) {
    // Предотвращаем деление на 0
    height = 1;
  }

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Вычисляем соотношение сторон окна
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 300.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int DrawGLScene(GLvoid) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(0.0, 5.0, -150.0, 0.0, 15.0, -130, 0, 1, 0);

  saluteManager.Update();
  saluteManager.Draw();

  return TRUE;
}

GLvoid KillGLWindow(bool bShowError) {
  if (fullscreen) {
    // Если да, то переключаемся на рабочий стол
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
  }

  if (hRC) {
    if (!wglMakeCurrent(NULL, NULL) && bShowError) {
      MessageBox(NULL, "Ошибка освобождения контекстов DC и RC.",
                 "Ошибка завершения", MB_OK | MB_ICONINFORMATION);
    }

    if (!wglDeleteContext(hRC) && bShowError) {
      MessageBox(NULL, "Ошибка удаления контекста рендеринга.",
                 "Ошибка завершения", MB_OK | MB_ICONINFORMATION);
    }
    hRC = NULL;
  }

  if (hDC && !ReleaseDC(hWnd, hDC) && bShowError) {
    MessageBox(NULL, "Ошибка удаления контекста устройства.",
               "Ошибка завершения", MB_OK | MB_ICONINFORMATION);
  }
  hDC = NULL;

  if (hWnd && !DestroyWindow(hWnd) && bShowError) {
    MessageBox(NULL, "Ошибка удаления окна.", "Ошибка завершения",
               MB_OK | MB_ICONINFORMATION);
  }
  hWnd = NULL;

  if (!UnregisterClass(wndName, hInstance) && bShowError) {
    MessageBox(NULL, "Ошибка разрегистрации класса окна.",
               "Ошибка завершения", MB_OK | MB_ICONINFORMATION);
  }
  hInstance = NULL;
}

/*  Эта часть программы создает наше окно SaluteWnd.  Параметры:
  title     - Заголовок, отображаемый наверху окна
  width     - Ширина GL окна или полноэкранного режима
  height      - Высота GL окна или полноэкранного режима
  bits      - Количество бит на цвет (8/16/24/32)
  fullScreenFlag  - Использование полноэкранного (TRUE) или оконного (FALSE) режимов  */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullScreenFlag) {
  GLuint    pixelFormat;
  WNDCLASS  wc;
  DWORD   dwExStyle;
  DWORD   dwStyle;
  RECT    windowRect;
  windowRect.left = (long)0;
  windowRect.right = (long)width;
  windowRect.top = (long)0;
  windowRect.bottom = (long)height;

  fullscreen = fullScreenFlag;

  hInstance = GetModuleHandle(NULL);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC) WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground  = NULL; // Фон для GL не требуется
  wc.lpszMenuName   = NULL; // Нам не нужны меню
  wc.lpszClassName  = wndName; // Установим имя класса

  if (!RegisterClass(&wc)) {
    MessageBox(NULL, "Ошибка регистрации класса окна.", "Ошибка",
               MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (fullscreen) {
    RECT rcDesktop;
    rcDesktop.right = width;
    rcDesktop.bottom = height;
    HWND hDesktop = ::GetDesktopWindow();
    if (hDesktop) {
      ::GetWindowRect(hDesktop, &rcDesktop);
      windowRect.right = rcDesktop.right = rcDesktop.right - rcDesktop.left;
      windowRect.bottom = rcDesktop.bottom = rcDesktop.bottom - rcDesktop.top;
      rcDesktop.left = rcDesktop.top = 0;
    }

    DEVMODE dmScreenSettings; // Режим устройства
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = rcDesktop.right;
    dmScreenSettings.dmPelsHeight = rcDesktop.bottom;
    dmScreenSettings.dmBitsPerPel = bits;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Попытаемся установить выбранный режим и получить результаты
    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      if (MessageBox(NULL,
                     "Полноэкранный режим не поддерживается видеокартой.\n Продолжить работу в оконном режиме?",
                     "Предупреждение", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        fullscreen = FALSE;
      }
      else {
        MessageBox(NULL, "Программа будет закрыта.", "Ошибка",
                   MB_OK | MB_ICONSTOP);
        return FALSE;
      }
    }
  }

  if (fullscreen) {
    dwExStyle = WS_EX_APPWINDOW;
    dwStyle = WS_POPUP;
    ShowCursor(FALSE);
  }
  else {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  // Создать окно
  if (!(hWnd = CreateWindowEx(dwExStyle, wndName, title, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                              0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                              NULL, NULL, hInstance, NULL))) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибка создания окна.", "Ошибка",
               MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  static  PIXELFORMATDESCRIPTOR pfd
  = {     // pfd сообщает Windows какие параметры мы хотим
    sizeof(PIXELFORMATDESCRIPTOR), // Размер дескриптора формата пикселей
    1,                             // Номер версии
    PFD_DRAW_TO_WINDOW |           // Формат должен поддерживать окно
    PFD_SUPPORT_OPENGL |           // Формат должен поддерживать OpenGL
    PFD_DOUBLEBUFFER,              // Должна поддерживаться двойная буферизация
    PFD_TYPE_RGBA,                 // Запрос RGBA формата
    bits,                          // Выбор глубины цвета
    0, 0, 0, 0, 0, 0,              // Биты цвета игнорируются
    0,                             // Нет альфа буферизации
    0,                             // Бит сдвига игнорируется
    0,                             // Нет буфера накопления
    0, 0, 0, 0,                    // Биты накопления игнорируются
    16,                            // 16-битный Z-буфер (Буфер глубины)
    0,                             // Нет буфера трафарета (stencil buffer)
    0,                             // Нет вспомогательного буфера
    PFD_MAIN_PLANE,                // Главная плоскость рисования
    0,                             // Зарезервировано
    0, 0, 0                        // Слой масок игнорируется
  };

  if (!(hDC = GetDC(hWnd))) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибка создания контекста устройства GL.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!(pixelFormat = ChoosePixelFormat(hDC, &pfd))) {
    KillGLWindow(false);
    MessageBox(NULL, "Не найден соответствующий формат пикселей.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!SetPixelFormat(hDC, pixelFormat, &pfd)) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибка установки формата пикселей",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!(hRC = wglCreateContext(hDC))) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибка создания контекста рендеринга GL.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!wglMakeCurrent(hDC, hRC)) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибка активации контекста рендеринга GL.",
               "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  ShowWindow(hWnd, SW_SHOW);
  SetForegroundWindow(hWnd);
  SetFocus(hWnd);
  ResizeGLScene(width, height);

  if (!InitGL()) {
    KillGLWindow(false);
    MessageBox(NULL, "Ошибька в инициализация.", "Ошибка",
               MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_ACTIVATE: {
      active = !HIWORD(wParam);
      return 0;
    }
    case WM_SYSCOMMAND: {
      switch (wParam) {
        case SC_SCREENSAVE:   // Пытается включиться скринсэйвер?
        case SC_MONITORPOWER: // Монитор пытается переключитться в режим сохранения энергии?
          return 0;           // Не давать этому случиться
      }
      break;
    }
    case WM_CLOSE: {
      PostQuitMessage(0);
      return 0;
    }
    case WM_KEYDOWN: {
      keys[wParam] = TRUE;
      return 0;
    }
    case WM_KEYUP: {
      keys[wParam] = FALSE;
      return 0;
    }
    case WM_SIZE: {
      ResizeGLScene(LOWORD(lParam), HIWORD(lParam));
      return 0;
    }
    case WM_TIMER: {
      if (active && !done) {
        if (DrawGLScene()) {
          SwapBuffers(hDC);
        }
        else {
          done = true;
        }
      }
      break;
    }
  }

  // Пересылаем все прочие сообщения в DefWindowProc
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  MSG msg;

  uint paricleCount = iniReader.GetParticlesCount();
  saluteManager.AddFireworks(paricleCount, 0, "CFireworksVolleySimple", "", "CGlParticle");
  saluteManager.AddFireworks(paricleCount, iniReader.GetSubParticlesCount(),
                             "CFireworksVolleyExplode", "CExplodeParticleSystem", "CGlParticle");
  saluteManager.AddFireworks(paricleCount, 0, "CFireworksVolleyDouble", "", "CGlParticle");

  if (!saluteManager.Init(iniReader.GetVolleysCount(), paricleCount, iniReader.GetInterval())) {
    return 0;
  }

  // Создадим наше окно OpenGL
  int width = iniReader.GetWidth();
  int height = iniReader.GetHeight();

  if (!CreateGLWindow("Салют. Автор: Владимир Хованский", width, height,
                      32, iniReader.GetFullscreen())) {
    return 0;
  }

  hTimer = SetTimer(hWnd, 0, 50, (TIMERPROC) NULL);

  // Main message loop:
  while (!done && GetMessage(&msg, NULL, 0, 0)) {
    if (msg.message == WM_SYSCOMMAND) {
      if ((msg.wParam & 0xFFF0) == SC_CLOSE) {
        continue;
      }
    }

    if (keys[VK_ESCAPE]) {
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  KillGLWindow(true);
  return (msg.wParam);
}
