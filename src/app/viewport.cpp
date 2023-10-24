#include "viewport.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

Viewport::Viewport(QWidget *parent) : QOpenGLWidget(parent) {
  //  setBackgroundColor(QColor(255, 0, 0, 127));
}

Viewport::~Viewport() { ; }

void Viewport::initializeGL() {
  // Set up the rendering context, load shaders and other resources, etc.

  initializeOpenGLFunctions();
  // set proper rendering of element overlay
  glEnable(GL_DEPTH_TEST);
}

void Viewport::resizeGL(int w, int h) {
  // Update projection matrix and other size related settings

  glViewport(0, 0, w, h);
  this->w = w;
  this->h = h;
  // m_projection.setToIdentity();
  // m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

static const float positions[] = {
    0.5,  -0.5, -0.5,  //
    0.5,  -0.5, -1,    //
    -0.5, -0.5, -0.5,  //
    -0.5, -0.5, -1,    //

    0.5,  0.5,  -0.5,  //
    0.5,  0.5,  -1,    //
    -0.5, 0.5,  -0.5,  //
    -0.5, 0.5,  -1,    //
};
static const float cols[] = {
    0.5,  -0.5, 0.5,  //
    0.5,  -0.5, 1,    //
    -0.5, -0.5, 0.5,  //
    -0.5, -0.5, 1,    //

    0.5,  0.5,  0.5,  //
    0.5,  0.5,  1,    //
    -0.5, 0.5,  0.5,  //
    -0.5, 0.5,  1,    //
};

static const unsigned int indices[] = {3, 2, 0, 1,   // bottom
                                       4, 5, 7, 6,   // top
                                       2, 3, 7, 6,   // left
                                       0, 1, 5, 4,   // right
                                       0, 4, 6, 2,   // near
                                       1, 3, 7, 5};  // far
static const unsigned int face_vertices_counts[] = {4, 4, 4, 4, 4, 4};

void Viewport::paintGL() {
  // Draw the scene:

  // set the background color
  //  glClearColor(0.99f, 0.99f, 0.97f, 0.5f);
  QColor bc = getBackgroundColor();
  glClearColor(bc.redF(), bc.greenF(), bc.blueF(), bc.alphaF());
  // clear the viewport by setting all the pixels to the background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(0.5, 0.5, 0.5);
  glTranslatef(0, 0, -1);
  glRotatef(mouse_rotx, 1, 0, 0);
  glRotatef(mouse_roty, 0, 1, 0);

  // set projection matrix
  glMatrixMode(GL_PROJECTION);
  float ar = (float)this->w / (float)this->h;
  glLoadIdentity();
  float l = -1;
  float r = 1;
  glOrtho((float)l / ar, (float)r / ar, -1, 1, 0, 2);

  unsigned int face_count = 6;
  unsigned int index_count = 8;

  // set the buffers
  glVertexPointer(3, GL_FLOAT, 0, positions);
  glEnableClientState(GL_VERTEX_ARRAY);
  //  glColorPointer(3, GL_FLOAT, 0, cols);
  //  glEnableClientState(GL_COLOR_ARRAY);

  // send the line draw calls
  QColor lc = getLineColor();
  glColor3d(lc.redF(), lc.greenF(), lc.blueF());
  if (this->is_line_drawing_active) {
    glLineWidth(6);
    const unsigned int *index_offset = &indices[0];
    for (unsigned int i = 0; i < face_count; i++) {
      glDrawElements(GL_LINE_LOOP, face_vertices_counts[i], GL_UNSIGNED_INT,
                     index_offset);
      index_offset += face_vertices_counts[i];
    }
  }

  // send the point draw calls
  QColor pc = getPointColor();
  glColor3d(pc.redF(), pc.greenF(), pc.blueF());
  if (this->is_point_drawing_active) {
    glPointSize(30);
    glDrawArrays(GL_POINTS, 0, index_count);
  }

  // disable the buffers
  //  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

// set interactive rotation in the viewport with a mouse
void Viewport::mousePressEvent(QMouseEvent *m) { mouse_pos = m->pos(); }
void Viewport::mouseMoveEvent(QMouseEvent *m) {
  mouse_rotx += mouse_speed * (m->pos().y() - mouse_pos.y());
  mouse_roty += mouse_speed * (m->pos().x() - mouse_pos.x());
  mouse_pos = m->pos();
  update();
}
