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

 private slots:
  void on_displayPointsCheckBox_stateChanged(int arg1);
  void on_displayLinesCheckBox_stateChanged(int arg1);
  void on_backgroundColorPushButton_clicked();
  void on_lineColorPushButton_clicked();
  void on_pointColorPushButton_clicked();
  void on_displayLinesCheckBox_toggled(bool checked);
  void on_displayPointsCheckBox_toggled(bool checked);

  void PaintLineColorButton();
  void PaintBackgroundColorButton();
  void PaintPointColorButton();
};

#endif  // MAINWINDOW_HPP
