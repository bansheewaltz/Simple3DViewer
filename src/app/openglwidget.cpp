#include "openglwidget.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  resetSettings();
}
OpenGLWidget::~OpenGLWidget() { ; }

void OpenGLWidget::resetSettings() {
  // Colors
  setBackgroundColor(QColor("#EFE5D7"));
  setLineColor(QColor("#974F4C"));
  setPointColor(getLineColor());
  //
  setLineWidth(3);
  // Primitives' style
  setPointStyle(PointStyle::CIRCLE);
  // Camera
  setCameraSpeed(0.2f);
  setRotX(-30);
  setRotY(-30);
  setRotZ(0);
}

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
  this->viewport_w = w;
  this->viewport_h = h;
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

  //  glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
  //  glRotatef(45, 0, 1, 0);
  //  glRotatef(camera_rotx, 1, 0, 0);
  //  glRotatef(camera_roty, 0, 1, 0);
  //  drawAxes();
  //  drawCube(0, 0, 0, 0.7);

  // Set the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(0.5, 0.5, 0.5);
  glTranslatef(0, 0, -1);
  glRotatef(camera_rotx, 1, 0, 0);
  glRotatef(camera_roty, 0, 1, 0);
  drawAxes();

  // Set projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float ar = (float)this->viewport_w / (float)this->viewport_h;
  glOrtho(-1 * ar, 1 * ar, -1, 1, 0, 2);

  // Paint the object
  drawCube(0, 0, 0, 1);
  float vspacing = 1.5;
  drawCube(0, +vspacing, 0, 0.33);
  drawCube(0, -vspacing, 0, 0.33);

  //  paintObject(this->mesh);
}

void OpenGLWidget::drawObject(const ObjViewerMesh &m) {
  // Set up the buffers
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m.positions);

  // Send the line draw calls
  if (this->isLineDisplayEnabled()) {
    QColor lc = getLineColor();
    glColor3d(lc.redF(), lc.greenF(), lc.blueF());
    float lw = getLineWidth();
    glLineWidth(lw);
    if (getLineStyle() == LineStyle::DASHED) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(10, 0x3333);
    }

    const unsigned int *index_offset = &m.indices[0];
    for (unsigned int i = 0; i < m.face_count; i++) {
      glDrawElements(GL_LINE_LOOP, m.face_vertex_counts[i], GL_UNSIGNED_INT,
                     index_offset);
      index_offset += m.face_vertex_counts[i];
    }

    glDisable(GL_LINE_STIPPLE);
  }

  // Send the point draw calls
  if (this->isPointDisplayEnabled()) {
    QColor pc = getPointColor();
    glColor3d(pc.redF(), pc.greenF(), pc.blueF());
    float ps = getPointSize();
    glPointSize(ps);
    if (getPointStyle() == PointStyle::CIRCLE) {
      glEnable(GL_POINT_SMOOTH);
    }

    glDrawArrays(GL_POINTS, 0, m.position_count);

    glDisable(GL_POINT_SMOOTH);
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
  drawObject(m);
}

void OpenGLWidget::drawAxes() {
  static float fmax = 1000;
  static float fmin = -fmax;

  static unsigned int position_count = 6;
  static float positions[] = {
      fmin, 0,    0,     // -x
      fmax, 0,    0,     // +x
      0,    fmin, 0,     // -y
      0,    fmax, 0,     // +y
      0,    0,    fmin,  // -z
      0,    0,    fmax,  // +z
  };
  //  static unsigned int face_count = 6;
  //  static unsigned int face_vertex_counts[] = {4, 4, 4, 4, 4, 4};
  static unsigned int index_count = 6;
  static unsigned int indices[] = {0, 1, 2, 3, 4, 5};

  glVertexPointer(3, GL_FLOAT, 0, positions);
  glLineWidth(1);
  glDrawArrays(GL_LINES, 0, position_count);
}

// Set interactive rotation in the viewport with a mouse
void OpenGLWidget::mousePressEvent(QMouseEvent *m) { mouse_pos = m->pos(); }
void OpenGLWidget::mouseMoveEvent(QMouseEvent *m) {
  camera_rotx += camera_speed * (m->pos().y() - mouse_pos.y());
  camera_roty += camera_speed * (m->pos().x() - mouse_pos.x());
  mouse_pos = m->pos();
  update();
}
