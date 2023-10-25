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
  //  ObjViewerMatrix4x4 model_matrix;
  //  ObjViewerMatrix4x4 view_matrix;
  //  ObjViewerMatrix4x4 projection_matrix;

  // Display settings
  QColor background_color;
  QColor line_color;
  QColor point_color;
  bool line_display_enabled;
  bool point_display_enabled;
  enum LineStyle { SOLID, DASHED };
  enum PointStyle { CIRCLE, SQUARE };
  LineStyle line_style;
  PointStyle point_style;
  float line_width;
  float point_size;
  // Mouse camera control
  float camera_speed;
  QPoint mouse_pos;
  float mouse_rotx;
  float mouse_roty;
  float mouse_rotz;
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
  void resetSettings();
  virtual void drawCube(float, float, float, float);
  virtual void paintObject(const ObjViewerMesh &);
  // Events
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

 public:
  // Primitives' Colors
  void setBackgroundColor(const QColor &color) { background_color = color; }
  void setLineColor(const QColor &color) { line_color = color; }
  void setPointColor(const QColor &color) { point_color = color; }
  QColor getBackgroundColor() const { return background_color; }
  QColor getLineColor() const { return line_color; }
  QColor getPointColor() const { return point_color; }
  // Primitives' width/size
  void setLineWidth(float width) { line_width = width; }
  void setPointSize(float size) { point_size = size; }
  float getLineWidth() const { return line_width; }
  float getPointSize() const { return point_size; }
  // Primitives' style
  void setLineStyle(LineStyle ls) { ; }
  void setPointStyle(PointStyle ps) { ; }
  // Camera
  void setCameraSpeed(float speed) { camera_speed = speed; }
  void setRotX(int angle = 0) { mouse_rotx = angle; }
  void setRotY(int angle = 0) { mouse_roty = angle; }
  void setRotZ(int angle = 0) { mouse_rotz = angle; }
  float getCameraSpees() const { return camera_speed; }
  float getRotX() const { return mouse_rotx; }
  float getRotY() const { return mouse_roty; }
  float getRotZ() const { return mouse_rotz; }
};

#endif  // OPENGLWIDGET_H
