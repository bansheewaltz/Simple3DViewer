#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>

#include "QtWidgets/qspinbox.h"
#include "doubleslider.hpp"
#include "openglwidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  void paintButton(QPushButton *b, QColor (OpenGLWidget::*getColor)() const);
  void setupLocationControls(DoubleSlider *, QDoubleSpinBox *);
  void updateLocation(double value, bool x, bool y, bool z);

 private slots:
  /* style */
  void on_backgroundColorPicker_clicked();
  void on_lineColorPicker_clicked();
  void on_pointColorPicker_clicked();
  void on_displayLinesCheckBox_toggled(bool checked);
  void on_displayPointsCheckBox_toggled(bool checked);
  void on_pointSizeSlider_valueChanged(int value);
  void on_pointStyleSquareCheckBox_toggled(bool checked);
  void on_lineWidthSlider_valueChanged(int value);
  void on_lineStyleDashedCheckBox_toggled(bool checked);
  /* Location */
  void on_xLocationSlider_doubleValueChanged(double value);
  void on_yLocationSlider_doubleValueChanged(double value);
  void on_zLocationSlider_doubleValueChanged(double value);
  void on_locationResetPushButton_clicked();
};

#endif  // MAINWINDOW_HPP
