#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>

#include "./ui_mainwindow.h"

enum ControlSteps {
  LOCATION = 100,
  ROTATION = 1800,
  SCALE = 100,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  /* Paint the color picker buttons */
  paintButton(ui->backgroundColorPicker, &OpenGLWidget::getBackgroundColor);
  paintButton(ui->lineColorPicker, &OpenGLWidget::getLineColor);
  paintButton(ui->pointColorPicker, &OpenGLWidget::getPointColor);
  /* Set primitives' display status */
  on_displayLinesCheckBox_toggled(true);
  on_displayPointsCheckBox_toggled(false);
  /* Set up location controls */
  setupLocationControls(ui->xLocationSlider, ui->xLocationSpinbox);
  setupLocationControls(ui->yLocationSlider, ui->yLocationSpinbox);
  setupLocationControls(ui->zLocationSlider, ui->zLocationSpinbox);
}

MainWindow::~MainWindow() { delete ui; }

/* GUI behaviour related helper functions */

QString formColoredButtonStyleSheet(const QColor &c) {
  QString qss = QString(
                    "background-color: %1; border-width: 2px;"
                    "border-style: solid; border-radius: 5px;")
                    .arg(c.name());
  return qss;
}

void MainWindow::paintButton(QPushButton *b,
                             QColor (OpenGLWidget::*getColor)() const) {
  QColor c = (ui->viewport->*getColor)();
  /* Update the button's QT stylesheet */
  QString qss = formColoredButtonStyleSheet(c);
  b->setStyleSheet(qss);
}

QColor convertColorToGreyscale(const QColor &c) {
  // formula from the documentaiton:
  // https://doc.qt.io/archives/qt-4.8/qcolor.html#qGray
  int greyv = (c.red() * 11 + c.green() * 16 + c.blue() * 5) / 32;
  /* Set the bounds */
  static const int lbound = 43 * (float)255 / 100;  // lower
  static const int ubound = 81 * (float)255 / 100;  // upper
  if (greyv < lbound) {
    greyv = lbound;
  }
  if (greyv > ubound) {
    greyv = ubound;
  }

  QColor result = QColor(greyv, greyv, greyv);
  return result;
}

void setLayoutWidgetsState(const QLayout *layout, bool state) {
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget != NULL) {
      widget->setEnabled(state);
    } else {
      QLayout *inner = layout->itemAt(i)->layout();
      setLayoutWidgetsState(inner, state);
    }
  }
}

/* GUI behaviour related signal functions */

void MainWindow::on_backgroundColorPicker_clicked() {
  QColor prev_color = ui->viewport->getBackgroundColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setBackgroundColor(color);
  ui->viewport->update();
  paintButton(ui->backgroundColorPicker, &OpenGLWidget::getBackgroundColor);
}
void MainWindow::on_lineColorPicker_clicked() {
  QColor prev_color = ui->viewport->getLineColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setLineColor(color);
  ui->viewport->update();
  paintButton(ui->lineColorPicker, &OpenGLWidget::getLineColor);
}
void MainWindow::on_pointColorPicker_clicked() {
  QColor prev_color = ui->viewport->getPointColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setPointColor(color);
  ui->viewport->update();
  paintButton(ui->pointColorPicker, &OpenGLWidget::getPointColor);
}

void MainWindow::on_displayLinesCheckBox_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setLineDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check */
  ui->displayLinesCheckBox->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->lineSettingsLayout, checked);
  ui->lineStyleDashedCheckBox->setEnabled(checked);
  // remember the color to use it when the button will be enabled next time
  static QColor color;
  color = ui->viewport->getLineColor();
  /* Update the color */
  QColor result = color;
  if (checked == false) {
    result = convertColorToGreyscale(color);
  }
  QString qss = formColoredButtonStyleSheet(result);
  ui->lineColorPicker->setStyleSheet(qss);
}
void MainWindow::on_displayPointsCheckBox_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setPointDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check */
  ui->displayPointsCheckBox->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->pointSettingsLayout, checked);
  ui->pointStyleSquareCheckBox->setEnabled(checked);
  // remember the color to use it when the button will be enabled next time
  static QColor color;
  color = ui->viewport->getPointColor();
  /* Update the color */
  QColor result = color;
  if (checked == false) {
    result = ::convertColorToGreyscale(color);
  }
  QString qss = formColoredButtonStyleSheet(result);
  ui->pointColorPicker->setStyleSheet(qss);
}

/* Primitives' display control related functions */

void MainWindow::on_pointSizeSlider_valueChanged(int value) {
  ui->viewport->setPointSize(value);
  ui->viewport->update();
}
void MainWindow::on_lineWidthSlider_valueChanged(int value) {
  ui->viewport->setLineWidth(value);
  ui->viewport->update();
}

/* Primitives' style related functions */

void MainWindow::on_pointStyleSquareCheckBox_toggled(bool checked) {
  if (checked == true)
    ui->viewport->setPointStyle(PointStyle::SQUARE);
  else
    ui->viewport->setPointStyle(PointStyle::CIRCLE);
  ui->viewport->update();
}
void MainWindow::on_lineStyleDashedCheckBox_toggled(bool checked) {
  if (checked == true)
    ui->viewport->setLineStyle(LineStyle::DASHED);
  else
    ui->viewport->setLineStyle(LineStyle::SOLID);
  ui->viewport->update();
}

/* Location related functions */

void MainWindow::setupLocationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  /* Connect the slider with the corresponding spinbox and vice versa*/
  connect(s, &DoubleSlider::doubleValueChanged, sb, &QDoubleSpinBox::setValue);
  connect(sb, &QDoubleSpinBox::valueChanged, s, &DoubleSlider::setDoubleValue);
  /* Set up the slider */
  s->setMinimum(-ControlSteps::LOCATION);
  s->setMaximum(+ControlSteps::LOCATION);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = ControlSteps::LOCATION;
  s->setValue(0);
  /* Set up the spinbox */
  const float sb_limit = 1.0f;
  const float sb_step = sb_limit / ControlSteps::LOCATION;
  sb->setSingleStep(sb_step);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
}
void MainWindow::updateLocation(double value, bool x, bool y, bool z) {
  if (x) ui->viewport->setTranslationX(value);
  if (y) ui->viewport->setTranslationY(value);
  if (z) ui->viewport->setTranslationZ(value);
  ui->viewport->update();
}
void MainWindow::on_xLocationSlider_doubleValueChanged(double value) {
  updateLocation(value, 1, 0, 0);
}
void MainWindow::on_yLocationSlider_doubleValueChanged(double value) {
  updateLocation(value, 0, 1, 0);
}
void MainWindow::on_zLocationSlider_doubleValueChanged(double value) {
  updateLocation(value, 0, 0, 1);
}
void MainWindow::on_locationResetPushButton_clicked() {
  ui->xLocationSlider->setValue(0);
  ui->yLocationSlider->setValue(0);
  ui->zLocationSlider->setValue(0);
}
