#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>

#include "./ui_mainwindow.h"

enum ControlSteps {
  LOCATION = 1000,
  ROTATION = 1800,
  SCALE = 100,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  /* Paint the color picker buttons */
  paintButton(ui->backgroundColorPicker, ui->viewport->getBackgroundColor());
  paintButton(ui->lineColorPicker, ui->viewport->getLineColor());
  paintButton(ui->pointColorPicker, ui->viewport->getPointColor());
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

void MainWindow::paintButton(QPushButton *b, const QColor &c) {
  QString qss = QString(
                    "background-color: %1; border-width: 2px;"
                    "border-style: solid; border-radius: 5px;")
                    .arg(c.name());
  b->setStyleSheet(qss);
}

QColor convertColorToGreyscale(const QColor &c) {
  // formula from the documentaiton:
  // https://doc.qt.io/archives/qt-4.8/qcolor.html#qGray
  int greyv = (c.red() * 11 + c.green() * 16 + c.blue() * 5) / 32;
  /* Set the bounds */
  static const int lbound = 0.43 * 255;
  static const int ubound = 0.81 * 255;
  if (greyv < lbound) greyv = lbound;
  if (greyv > ubound) greyv = ubound;
  return QColor(greyv, greyv, greyv);
}

void setLayoutWidgetsState(const QLayout *layout, bool state) {
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    if (widget != NULL)
      widget->setEnabled(state);
    else {
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
  paintButton(ui->backgroundColorPicker, color);
}
void MainWindow::on_lineColorPicker_clicked() {
  QColor prev_color = ui->viewport->getLineColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setLineColor(color);
  ui->viewport->update();
  paintButton(ui->lineColorPicker, color);
}
void MainWindow::on_pointColorPicker_clicked() {
  QColor prev_color = ui->viewport->getPointColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  if (!color.isValid()) return;
  ui->viewport->setPointColor(color);
  ui->viewport->update();
  paintButton(ui->pointColorPicker, color);
}

void MainWindow::on_displayLinesCheckBox_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setLineDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check state */
  ui->displayLinesCheckBox->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->lineSettingsLayout, checked);
  ui->lineStyleDashedCheckBox->setEnabled(checked);
  /* Update the button color */
  QColor res_color = ui->viewport->getLineColor();
  if (checked == false) {
    res_color = convertColorToGreyscale(res_color);
  }
  paintButton(ui->lineColorPicker, res_color);
}
void MainWindow::on_displayPointsCheckBox_toggled(bool checked) {
  /* Set the state in the viewport */
  ui->viewport->setPointDisplayEnabled(checked);
  ui->viewport->update();
  /* Set the checkbox check state */
  ui->displayPointsCheckBox->setChecked(checked);
  /* Disable the settings' frame */
  setLayoutWidgetsState(ui->pointSettingsLayout, checked);
  ui->pointStyleSquareCheckBox->setEnabled(checked);
  /* Update the button color */
  QColor res_color = ui->viewport->getPointColor();
  if (checked == false) {
    res_color = convertColorToGreyscale(res_color);
  }
  paintButton(ui->pointColorPicker, res_color);
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
  ui->viewport->setPointStyle(checked == true ? PointStyle::SQUARE
                                              : PointStyle::CIRCLE);
  ui->viewport->update();
}
void MainWindow::on_lineStyleDashedCheckBox_toggled(bool checked) {
  ui->viewport->setLineStyle(checked == true ? LineStyle::DASHED
                                             : LineStyle::SOLID);
  ui->viewport->update();
}

/* Location related functions */

void MainWindow::setupLocationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  /* Connect the slider with the corresponding spinbox and vice versa*/
  connect(s, &DoubleSlider::doubleValueChanged, sb, &QDoubleSpinBox::setValue);
  connect(sb, &QDoubleSpinBox::valueChanged, s, &DoubleSlider::setDoubleValue);
  const unsigned int steps_count = ControlSteps::LOCATION;
  /* Set up the slider */
  s->setMinimum(-steps_count);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count;
  s->setValue(0);
  /* Set up the spinbox */
  const float sb_limit = 1.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(3);
  sb->setMinimumWidth(62);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
}
void MainWindow::on_xLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationX(value);
  ui->viewport->update();
}
void MainWindow::on_yLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationY(value);
  ui->viewport->update();
}
void MainWindow::on_zLocationSlider_doubleValueChanged(double value) {
  ui->viewport->setTranslationZ(value);
  ui->viewport->update();
}
void MainWindow::on_locationResetPushButton_clicked() {
  ui->xLocationSlider->setValue(0);
  ui->yLocationSlider->setValue(0);
  ui->zLocationSlider->setValue(0);
}
