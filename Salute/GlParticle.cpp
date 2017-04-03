#include "StdAfx.h"
#include "GlParticle.h"

#include <gl\gl.h>      // Заголовочный файл библиотеки OpenGL32
#include <gl\glu.h>     // Заголовочный файл библиотеки GLu32

CREATE_PARTICLE(CGlParticle);

CGlParticle::CGlParticle(void) {
}

CGlParticle::~CGlParticle(void) {
}

void CGlParticle::DrawMe() {
  glColor4f(m_oColor.r, m_oColor.g, m_oColor.b, sqrtf(m_fLife));

  switch (m_iType) {
    case D_TRIANGLE: {
      float fWidth = 0.5f;
      glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(m_oPosition.x + fWidth, m_oPosition.y + fWidth, m_oPosition.z);
      glVertex3f(m_oPosition.x - fWidth, m_oPosition.y + fWidth, m_oPosition.z);
      glVertex3f(m_oPosition.x + fWidth, m_oPosition.y - fWidth, m_oPosition.z);
      glVertex3f(m_oPosition.x - fWidth, m_oPosition.y - fWidth, m_oPosition.z);
      glEnd();
    }
    break;
    case D_LINE: {
      float shine[4] = {1.0f, 1.0f, 1.0f, 0.5f};
      glLineWidth(m_fSize);
      glBegin(GL_LINES);
      glColor4fv(m_oColor);
      glVertex3f(m_oPosition.x, m_oPosition.y, m_oPosition.z);
      glColor4fv (shine);
      CVector pos2 = m_oPosition - m_oDirection;
      glVertex3fv(pos2);
      glEnd();
    }
    break;
    case D_POINT_RAND: {
      if (rand() % 2) {
        glPointSize(m_fSize);
        glColor4fv(m_oColor);
        glBegin(GL_POINTS);
        glVertex3fv(m_oPosition);
        glEnd();
      }
    }
    case D_POINT: {
      glPointSize(m_fSize);
      glBegin(GL_POINTS);
      glVertex3f(m_oPosition.x, m_oPosition.y, m_oPosition.z);
      glEnd();
    }
    break;
  }
}

