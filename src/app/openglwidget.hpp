#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "glm/glm.hpp"
#include "obj_viewer.h"

enum LineStyle { SOLID, DASHED };
enum PointStyle { CIRCLE, SQUARE };

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 private:
  std::string file_name;
  ObjViewerMesh *mesh;
  //  ObjViewerMeshInfo *mesh_info;
  ObjViewerVec3 geometry_centre;
  /* Affine transformations */
  // translates to the world origin and scales to 1x1x1 cube
  //  ObjViewerMatrix4x4 norm_matrix;
  glm::mat4 norm_matrix;
  // model matrix transformations
  float scalex, scaley, scalez, scaleu;
  float rotx, roty, rotz;
  float trnsx, trnsy, trnsz;
  ObjViewerMatrix4x4 model_matrix;
  ObjViewerMatrix4x4 view_matrix;
  /* Display settings */
  QColor background_color;
  QColor line_color;
  QColor point_color;
  bool line_display_enabled;
  bool point_display_enabled;
  LineStyle line_style;
  PointStyle point_style;
  float line_width;
  float point_size;
  /* Camera control */
  float camera_speed;
  QPoint mouse_pos;
  float camera_rotx, camera_roty, camera_rotz;
  /* Viewport settings */
  float ar;  // aspect raio

 public:
  OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget();

 private:
  /* Template functions */
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  /* Helpers */
  void resetSettings();
  virtual void drawAxes();
  virtual void drawObject(const ObjViewerMesh *);
  virtual void drawCube(float, float, float, float);

  void drawCubeScene();
  /* Events */
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

 public:
  void setFileName(std::string f) { file_name = f; }
  void LoadModel();
  /* Object transformation */
  void setScaleX(float factor = 1) { scalex = factor; }
  void setScaleY(float factor = 1) { scaley = factor; }
  void setScaleZ(float factor = 1) { scalez = factor; }
  void setScaleU(float factor = 1) { scaleu = factor; }
  float getScaleX() const { return scalex; }
  float getScaleY() const { return scaley; }
  float getScaleZ() const { return scalez; }
  float getScaleU() const { return scaleu; }
  float getScaleUX() const { return scaleu * scalex; }
  float getScaleUY() const { return scaleu * scaley; }
  float getScaleUZ() const { return scaleu * scalez; }
  void setRotationX(float angle = 0) { rotx = angle; }
  void setRotationY(float angle = 0) { roty = angle; }
  void setRotationZ(float angle = 0) { rotz = angle; }
  float getRotationX() const { return rotx; }
  float getRotationY() const { return roty; }
  float getRotationZ() const { return rotz; }
  void setTranslationX(float value = 0) { trnsx = value; }
  void setTranslationY(float value = 0) { trnsy = value; }
  void setTranslationZ(float value = 0) { trnsz = value; }
  float getTranslationX() const { return trnsx; }
  float getTranslationY() const { return trnsy; }
  float getTranslationZ() const { return trnsz; }
  /* Primitives' display status */
  void setLineDisplayEnabled(bool e = true) { line_display_enabled = e; }
  void setPointDisplayEnabled(bool e = true) { point_display_enabled = e; }
  bool isLineDisplayEnabled() const { return line_display_enabled; }
  bool isPointDisplayEnabled() const { return point_display_enabled; }
  /* Primitives' colors */
  void setBackgroundColor(const QColor &color) { background_color = color; }
  void setLineColor(const QColor &color) { line_color = color; }
  void setPointColor(const QColor &color) { point_color = color; }
  QColor getBackgroundColor() const { return background_color; }
  QColor getLineColor() const { return line_color; }
  QColor getPointColor() const { return point_color; }
  /* Primitives' width/size */
  void setLineWidth(float width = 1) { line_width = width; }
  void setPointSize(float size = 1) { point_size = size; }
  float getLineWidth() const { return line_width; }
  float getPointSize() const { return point_size; }
  /* Primitives' style */
  void setLineStyle(LineStyle ls = LineStyle::SOLID) { line_style = ls; }
  void setPointStyle(PointStyle ps = PointStyle::CIRCLE) { point_style = ps; }
  LineStyle getLineStyle() const { return line_style; }
  PointStyle getPointStyle() const { return point_style; }
  /* Camera settings */
  void setCameraSpeed(float speed = 0.2) { camera_speed = speed; }
  void setCameraRotationX(float angle = 0) { camera_rotx = angle; }
  void setCameraRotationY(float angle = 0) { camera_roty = angle; }
  void setCameraRotationZ(float angle = 0) { camera_rotz = angle; }
  float getCameraSpeed() const { return camera_speed; }
  float getCameraRotationX() const { return camera_rotx; }
  float getCameraRotationY() const { return camera_roty; }
  float getCameraRotationZ() const { return camera_rotz; }
};

#endif  // OPENGLWIDGET_H
