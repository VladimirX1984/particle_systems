
#include "stdafx.h"

#include <gl\gl.h>      // ������������ ���� ���������� OpenGL32
#include <gl\glu.h>     // ������������ ���� ���������� GLu32

#include "Salute.h"
#include "SaluteManager.h"
#include "IniReader.h"
//#include "GlParticle.h"

HDC     hDC = NULL;  // ������� �������� GDI ����������
HGLRC   hRC = NULL; // ���������� �������� �����������
HWND    hWnd = NULL;
HINSTANCE hInstance;
UINT_PTR  hTimer = 0;

BOOL done = FALSE; // ��������� ���������� ������ �� �����
bool keys[256];    // ������ ����������� ��� ������������ ����������
bool active = true; // ���� "��������" ����. �� ��������� true.
bool fullscreen = false;

const char* wndName = "SaluteWnd";

CSaluteManager saluteManager; // �������� ������
CIniReader iniReader; // �������� ���������� �� ini-������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int InitGL(GLvoid) {
  glShadeModel(GL_SMOOTH); // ��������� ������� ���������
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
    // ������������� ������� �� 0
    height = 1;
  }

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // ��������� ����������� ������ ����
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
    // ���� ��, �� ������������� �� ������� ����
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
  }

  if (hRC) {
    if (!wglMakeCurrent(NULL, NULL) && bShowError) {
      MessageBox(NULL, "������ ������������ ���������� DC � RC.",
                 "������ ����������", MB_OK | MB_ICONINFORMATION);
    }

    if (!wglDeleteContext(hRC) && bShowError) {
      MessageBox(NULL, "������ �������� ��������� ����������.",
                 "������ ����������", MB_OK | MB_ICONINFORMATION);
    }
    hRC = NULL;
  }

  if (hDC && !ReleaseDC(hWnd, hDC) && bShowError) {
    MessageBox(NULL, "������ �������� ��������� ����������.",
               "������ ����������", MB_OK | MB_ICONINFORMATION);
  }
  hDC = NULL;

  if (hWnd && !DestroyWindow(hWnd) && bShowError) {
    MessageBox(NULL, "������ �������� ����.", "������ ����������",
               MB_OK | MB_ICONINFORMATION);
  }
  hWnd = NULL;

  if (!UnregisterClass(wndName, hInstance) && bShowError) {
    MessageBox(NULL, "������ �������������� ������ ����.",
               "������ ����������", MB_OK | MB_ICONINFORMATION);
  }
  hInstance = NULL;
}

/*  ��� ����� ��������� ������� ���� ���� SaluteWnd.  ���������:
  title     - ���������, ������������ ������� ����
  width     - ������ GL ���� ��� �������������� ������
  height      - ������ GL ���� ��� �������������� ������
  bits      - ���������� ��� �� ���� (8/16/24/32)
  fullScreenFlag  - ������������� �������������� (TRUE) ��� �������� (FALSE) �������  */

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
  wc.hbrBackground  = NULL; // ��� ��� GL �� ���������
  wc.lpszMenuName   = NULL; // ��� �� ����� ����
  wc.lpszClassName  = wndName; // ��������� ��� ������

  if (!RegisterClass(&wc)) {
    MessageBox(NULL, "������ ����������� ������ ����.", "������",
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

    DEVMODE dmScreenSettings; // ����� ����������
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = rcDesktop.right;
    dmScreenSettings.dmPelsHeight = rcDesktop.bottom;
    dmScreenSettings.dmBitsPerPel = bits;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // ���������� ���������� ��������� ����� � �������� ����������
    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      if (MessageBox(NULL,
                     "������������� ����� �� �������������� �����������.\n ���������� ������ � ������� ������?",
                     "��������������", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
        fullscreen = FALSE;
      }
      else {
        MessageBox(NULL, "��������� ����� �������.", "������",
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

  // ������� ����
  if (!(hWnd = CreateWindowEx(dwExStyle, wndName, title, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                              0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                              NULL, NULL, hInstance, NULL))) {
    KillGLWindow(false);
    MessageBox(NULL, "������ �������� ����.", "������",
               MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  static  PIXELFORMATDESCRIPTOR pfd
  = {     // pfd �������� Windows ����� ��������� �� �����
    sizeof(PIXELFORMATDESCRIPTOR), // ������ ����������� ������� ��������
    1,                             // ����� ������
    PFD_DRAW_TO_WINDOW |           // ������ ������ ������������ ����
    PFD_SUPPORT_OPENGL |           // ������ ������ ������������ OpenGL
    PFD_DOUBLEBUFFER,              // ������ �������������� ������� �����������
    PFD_TYPE_RGBA,                 // ������ RGBA �������
    bits,                          // ����� ������� �����
    0, 0, 0, 0, 0, 0,              // ���� ����� ������������
    0,                             // ��� ����� �����������
    0,                             // ��� ������ ������������
    0,                             // ��� ������ ����������
    0, 0, 0, 0,                    // ���� ���������� ������������
    16,                            // 16-������ Z-����� (����� �������)
    0,                             // ��� ������ ��������� (stencil buffer)
    0,                             // ��� ���������������� ������
    PFD_MAIN_PLANE,                // ������� ��������� ���������
    0,                             // ���������������
    0, 0, 0                        // ���� ����� ������������
  };

  if (!(hDC = GetDC(hWnd))) {
    KillGLWindow(false);
    MessageBox(NULL, "������ �������� ��������� ���������� GL.",
               "������", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!(pixelFormat = ChoosePixelFormat(hDC, &pfd))) {
    KillGLWindow(false);
    MessageBox(NULL, "�� ������ ��������������� ������ ��������.",
               "������", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!SetPixelFormat(hDC, pixelFormat, &pfd)) {
    KillGLWindow(false);
    MessageBox(NULL, "������ ��������� ������� ��������",
               "������", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!(hRC = wglCreateContext(hDC))) {
    KillGLWindow(false);
    MessageBox(NULL, "������ �������� ��������� ���������� GL.",
               "������", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  if (!wglMakeCurrent(hDC, hRC)) {
    KillGLWindow(false);
    MessageBox(NULL, "������ ��������� ��������� ���������� GL.",
               "������", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }

  ShowWindow(hWnd, SW_SHOW);
  SetForegroundWindow(hWnd);
  SetFocus(hWnd);
  ResizeGLScene(width, height);

  if (!InitGL()) {
    KillGLWindow(false);
    MessageBox(NULL, "������� � �������������.", "������",
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
        case SC_SCREENSAVE:   // �������� ���������� �����������?
        case SC_MONITORPOWER: // ������� �������� �������������� � ����� ���������� �������?
          return 0;           // �� ������ ����� ���������
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

  // ���������� ��� ������ ��������� � DefWindowProc
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

  // �������� ���� ���� OpenGL
  int width = iniReader.GetWidth();
  int height = iniReader.GetHeight();

  if (!CreateGLWindow("�����. �����: �������� ���������", width, height,
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
