#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>

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
  void paintButton(QPushButton *b, QColor (OpenGLWidget::*getColor)());
  void PaintBackgroundColorButton();
  void PaintLineColorButton();
  void PaintPointColorButton();

 private slots:
  void on_backgroundColorPushButton_clicked();
  void on_lineColorPushButton_clicked();
  void on_pointColorPushButton_clicked();
  void on_displayLinesCheckBox_toggled(bool checked);
  void on_displayPointsCheckBox_toggled(bool checked);
  void on_pointSizeSlider_valueChanged(int value);
  void on_pointStyleSquareCheckBox_toggled(bool checked);
  void on_lineWidthSlider_valueChanged(int value);
  void on_lineStyleDashedCheckBox_toggled(bool checked);
  void on_xLocationSlider_valueChanged(int value);
  void on_yLocationSlider_valueChanged(int value);
  void on_zLocationSlider_valueChanged(int value);
};

#endif  // MAINWINDOW_HPP
