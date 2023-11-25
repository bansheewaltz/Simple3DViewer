#include "openglwidget.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#include <iostream>

#include "owviewer.h"

#define VIEWCUBE_SIDE 2.0f

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  resetSettings();
}
OpenGLWidget::~OpenGLWidget() { owv_mesh_destroy((OWV_Mesh *)this->mesh); }

void OpenGLWidget::resetSettings() {
  this->mesh = nullptr;
  this->index_array = nullptr;
  //  this->face_index_list = nullptr;
  /* Colors */
  setBackgroundColor(QColor("#EFE5D7"));
  setLineColor(QColor("#974F4C"));
  setPointColor(getLineColor());
  /* Camera */
  setCameraSpeed(0.2f);
  setCameraRotationX(+30);
  setCameraRotationY(+40);
  setCameraRotationZ(0);
}

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}
static void GLCheckError() {
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL error: " << error << std::endl;
  }
}

/* Set up the rendering context, load shaders and other resources, etc. */
void OpenGLWidget::initializeGL() {
  /* Retrieve OpenGL functions from graphics card's drivers */
  initializeOpenGLFunctions();
  /* Set proper rendering of overlapping elements */
  glEnable(GL_DEPTH_TEST);
}

/* Update the viewport size */
void OpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  this->ar = (float)w / (float)h;
}

/* Draw the scene */
void OpenGLWidget::paintGL() {
  /* Set the background color */
  QColor bc = getBackgroundColor();
  glClearColor(bc.redF(), bc.greenF(), bc.blueF(), bc.alphaF());
  /* Clear the buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* Set the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection_type == ProjectionType::PERSPECTIVE) {
    glFrustum(-0.5 * this->ar, 0.5 * this->ar, -0.5, 0.5, 1, 1000);
    glTranslatef(0, 0, -2 * this->ar);
  } else {
    glOrtho(-1 * this->ar, 1 * this->ar, -1, 1, -1, 1);
  }

  /* Draw the world axes */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // view rotation
  glRotatef(getCameraRotationX(), 1, 0, 0);
  glRotatef(getCameraRotationY(), 0, 1, 0);
  drawAxes();

  /* Set the model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // view rotation
  glRotatef(getCameraRotationX(), 1, 0, 0);
  glRotatef(getCameraRotationY(), 0, 1, 0);
  // view scale
  glScalef(0.7f, 0.7f, 0.7f);
  // model translation
  glTranslatef(getTranslationX(), getTranslationY(), getTranslationZ());
  // model rotation
  glRotatef(getRotationX(), 1, 0, 0);
  glRotatef(getRotationY(), 0, 1, 0);
  glRotatef(getRotationZ(), 0, 0, 1);
  // model scale
  glScalef(getScaleUX(), getScaleUY(), getScaleUZ());

  /* Draw the objects */
  if (!mesh)  // use a template model
    drawCubeScene();
  else
    drawObject(mesh);
}

void OpenGLWidget::drawCubeScene() {
  drawAxes();
  drawCube(0, 0, 0, 1);
  //  const static float vspacing = 1.5;
  //  drawCube(0, +vspacing, 0, 0.33);
  //  drawCube(0, -vspacing, 0, 0.33);
}

void OpenGLWidget::drawObject(const OWV_Mesh *m) {
  if (!m) return;
  /* Centre the object */
  //! make the norm matrix out of this set of transformations
  const float maxlen = mesh_bounds.maxlen;
  const float vcs = VIEWCUBE_SIDE;
  glScalef(vcs / maxlen, vcs / maxlen, vcs / maxlen);
  glTranslatef(-mesh_bounds.xcen, -mesh_bounds.ycen, -mesh_bounds.zcen);

  /* Set up the buffers */
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m->positions);

  /* Send the line draw calls */
  if (this->isLineDisplayEnabled()) {
    QColor lc = getLineColor();
    glColor3d(lc.redF(), lc.greenF(), lc.blueF());
    float lw = getLineWidth();
    glLineWidth(lw);
    if (getLineStyle() == LineStyle::DASHED) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(10, 0x3333);
    }

    /* GL_LINE_LOOP glDrawElements in a loop variation */
    //    const unsigned int *index_offset = &m->indices[0];
    //    for (unsigned int i = 0; i < m->face_count; i++) {
    //      glDrawElements(GL_LINE_LOOP, m->face_vertex_counts[i],
    //      GL_UNSIGNED_INT,
    //                     index_offset);
    //      index_offset += m->face_vertex_counts[i];
    //    }

    /* GL_LINE_LOOP glMultiDrawElements variation */
    //    glMultiDrawElements(GL_LINE_LOOP, (GLsizei *)m->face_vertex_counts,
    //                        GL_UNSIGNED_INT, (GLvoid **)face_index_list,
    //                        m->face_count);

    /* GL_LINES glDrawElements variation */
    glDrawElements(GL_LINES, this->index_count, GL_UNSIGNED_INT,
                   this->index_array);
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

    glDrawArrays(GL_POINTS, 0, m->vertex_count);

    glDisable(GL_POINT_SMOOTH);
  }

  //  glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGLWidget::drawCube(float x, float y, float z, float side_len) {
  const OWV_Mesh *m = owv_mesh_create_cube(x, y, z, side_len);
  this->mesh_bounds = {.xcen = 0, .ycen = 0, .zcen = 0, .maxlen = side_len * 2};
  this->index_array = owv_iarr_to_unique_lines(m, &this->index_count);
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
  static unsigned int index_count = 6;
  static unsigned int indices[] = {0, 1, 2, 3, 4, 5};

  glVertexPointer(3, GL_FLOAT, 0, positions);
  glLineWidth(1);
  glDrawArrays(GL_LINES, 0, position_count);
}

/* Set interactive rotation in the viewport with a mouse */
void OpenGLWidget::mousePressEvent(QMouseEvent *m) { mouse_pos = m->pos(); }
void OpenGLWidget::mouseMoveEvent(QMouseEvent *m) {
  camera_rotx += camera_speed * (m->pos().y() - mouse_pos.y());
  camera_roty += camera_speed * (m->pos().x() - mouse_pos.x());
  mouse_pos = m->pos();
  update();
}

void OpenGLWidget::loadModel() {
  OWV_Mesh *m = owv_mesh_read_obj(this->file_name.c_str());
  if (this->mesh) {
    owv_mesh_destroy((OWV_Mesh *)this->mesh);
  }
  this->mesh = m;
  if (this->index_array) {
    free(this->index_array);
    index_count = 0;
  }
  this->mesh_bounds = owv_mesh_find_bounds(m);
  /* Break the index array of faces into the index array of UNIQUE lines */
  this->index_array = owv_iarr_to_unique_lines(m, &this->index_count);
  this->faces_count = m->face_count;
  this->vertices_count = m->vertex_count - 1;
  this->edges_count = this->index_count / 2;
  /* Break the index array of faces into the array of index arrays by faces */
  //  owv_index_arr_to_2d_arr(m);
  /* Break the index array of faces into the index array of lines */
  //  this->index_array = owv_to_lines_index_arr(m);
  //  this->index_count = m->index_count * 2;
  update();
}
