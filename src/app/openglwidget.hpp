#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "obj_viewer.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 private:
  ObjViewerMesh mesh;
  //  ObjViewerMatrix4x4 m_model;
  //  ObjViewerMatrix4x4 m_view;
  //  ObjViewerMatrix4x4 m_projection;

  // Display settings
  QColor background_color = QColor("#EFE5D7");
  QColor line_color = QColor("#974F4C");
  QColor point_color = line_color;
  float line_width;
  float point_size;
  // Mouse camera control
  float camera_speed = 0.2;
  QPoint mouse_pos;
  float mouse_rotx = -30;
  float mouse_roty = -30;
  float mouse_rotz = 0;
  // Window settings
  int w, h;

 public:
  bool is_point_drawing_active;
  bool is_line_drawing_active;

 public:
  OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget();

 private:
  // Template functions
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  // Helpers
  void paintObject(const ObjViewerMesh &);
  void drawCube(float, float, float, float);
  // Events
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

 public:
  // Color
  void setBackgroundColor(const QColor &color) { background_color = color; }
  void setLineColor(const QColor &color) { line_color = color; }
  void setPointColor(const QColor &color) { point_color = color; }
  QColor getBackgroundColor() const { return background_color; }
  QColor getLineColor() const { return line_color; }
  QColor getPointColor() const { return point_color; }
  // Primitives' width/size
  void setPointSize(float size) { point_size = size; }
  void setLineWidth(float width) { line_width = width; }
  float getPointSize() const { return point_size; }
  float getLineWidth() const { return line_width; }
};

#endif  // OPENGLWIDGET_H
