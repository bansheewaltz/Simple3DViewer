#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "openglwidget.hpp"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) { ; }
OpenGLWidget::~OpenGLWidget() { ; }

void OpenGLWidget::initializeGL() {
  // Set up the rendering context, load shaders and other resources, etc.

  // Retrieve OpenGL functions from graphics card's drivers
  initializeOpenGLFunctions();
  // Set proper rendering of overlappint elements
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {
  // Update projection matrix and other size related settings

  glViewport(0, 0, w, h);
  this->w = w;
  this->h = h;
  // m_projection.setToIdentity();
  // m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void OpenGLWidget::paintGL() {
  // Draw the scene

  // Set the background color
  QColor bc = getBackgroundColor();
  glClearColor(bc.redF(), bc.greenF(), bc.blueF(), bc.alphaF());
  // clear the viewport by setting all the pixels to the background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(0.5, 0.5, 0.5);
  glTranslatef(0, 0, -1);
  glRotatef(mouse_rotx, 1, 0, 0);
  glRotatef(mouse_roty, 0, 1, 0);

  // Set projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float ar = (float)this->w / (float)this->h;
  glOrtho(-1 * ar, 1 * ar, -1, 1, 0, 2);

  // Paint the object
  drawCube(0, 0, 0, 1);
  drawCube(0, +1.5, 0, 0.33);
  drawCube(0, -1.5, 0, 0.33);
  //  paintObject(this->mesh);
}

void OpenGLWidget::paintObject(const ObjViewerMesh &m) {
  // Set up the buffers
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m.positions);

  // Send the line draw calls
  if (this->is_line_drawing_active) {
    QColor lc = getLineColor();
    glColor3d(lc.redF(), lc.greenF(), lc.blueF());
    glLineWidth(6);

    const unsigned int *index_offset = &m.indices[0];
    for (unsigned int i = 0; i < m.face_count; i++) {
      glDrawElements(GL_LINE_LOOP, m.face_vertex_counts[i], GL_UNSIGNED_INT,
                     index_offset);
      index_offset += m.face_vertex_counts[i];
    }
  }

  // Send the point draw calls
  if (this->is_point_drawing_active) {
    QColor pc = getPointColor();
    glColor3d(pc.redF(), pc.greenF(), pc.blueF());
    glPointSize(30);

    glDrawArrays(GL_POINTS, 0, m.position_count);
  }

  //  glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGLWidget::drawCube(float x, float y, float z, float side_len) {
  const float hside = side_len / 2;
  static unsigned int position_count = 8;
  float positions[] = {
      x + hside, y - hside, z + hside,  // right bottom front
      x + hside, y - hside, z - hside,  // right bottom back
      x - hside, y - hside, z + hside,  // left bottom front
      x - hside, y - hside, z - hside,  // left bottom back

      x + hside, y + hside, z + hside,  // right top front
      x + hside, y + hside, z - hside,  // right top back
      x - hside, y + hside, z + hside,  // left top front
      x - hside, y + hside, z - hside,  // left top back
  };
  static unsigned int face_count = 6;
  static unsigned int face_vertex_counts[] = {4, 4, 4, 4, 4, 4};
  static unsigned int index_count = 8;
  static unsigned int indices[] = {3, 2, 0, 1,   // bottom
                                   4, 5, 7, 6,   // top
                                   2, 3, 7, 6,   // left
                                   0, 1, 5, 4,   // right
                                   0, 4, 6, 2,   // near
                                   1, 3, 7, 5};  // far
  ObjViewerMesh m = {position_count,     positions,   face_count,
                     face_vertex_counts, index_count, indices};
  paintObject(m);
}

// Set interactive rotation in the viewport with a mouse
void OpenGLWidget::mousePressEvent(QMouseEvent *m) { mouse_pos = m->pos(); }
void OpenGLWidget::mouseMoveEvent(QMouseEvent *m) {
  mouse_rotx += camera_speed * (m->pos().y() - mouse_pos.y());
  mouse_roty += camera_speed * (m->pos().x() - mouse_pos.x());
  mouse_pos = m->pos();
  update();
}
