#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>

#include "QtWidgets/qspinbox.h"
#include "doubleslider.hpp"

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
  void resetSettings();
  void paintButton(QPushButton *b, const QColor &c);
  void setupWidthControls(DoubleSlider *, QDoubleSpinBox *);
  void setupLocationControls(DoubleSlider *, QDoubleSpinBox *);
  void setupRotationControls(DoubleSlider *, QDoubleSpinBox *);
  void setupScaleControls(DoubleSlider *, QDoubleSpinBox *);

 private slots:
  void openFile();
  /* Display */
  void on_backgroundColorPicker_clicked();
  void on_lineColorPicker_clicked();
  void on_pointColorPicker_clicked();
  void on_displayLinesCheckBox_toggled(bool checked);
  void on_displayPointsCheckBox_toggled(bool checked);
  void on_pointSizeSlider_doubleValueChanged(double value);
  void on_pointStyleSquareCheckBox_toggled(bool checked);
  void on_lineWidthSlider_doubleValueChanged(double value);
  void on_lineStyleDashedCheckBox_toggled(bool checked);
  /* Location */
  void on_xLocationSlider_doubleValueChanged(double value);
  void on_yLocationSlider_doubleValueChanged(double value);
  void on_zLocationSlider_doubleValueChanged(double value);
  void on_locationResetPushButton_clicked();
  /* Rotation */
  void on_xRotationSlider_doubleValueChanged(double angle);
  void on_yRotationSlider_doubleValueChanged(double angle);
  void on_zRotationSlider_doubleValueChanged(double angle);
  void on_rotationResetPushButton_clicked();
  /* Scale */
  void on_xScaleSlider_doubleValueChanged(double factor);
  void on_yScaleSlider_doubleValueChanged(double factor);
  void on_zScaleSlider_doubleValueChanged(double factor);
  void on_uScaleSlider_doubleValueChanged(double factor);
  void on_scaleResetPushButton_clicked();
};

#endif  // MAINWINDOW_HPP
