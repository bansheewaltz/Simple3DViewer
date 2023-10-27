#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>

#include "./ui_mainwindow.h"

// void changeLabelPalette(QWidget *l);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  PaintLineColorButton();
  PaintBackgroundColorButton();
  PaintPointColorButton();

  // Set lines display status
  on_displayLinesCheckBox_toggled(true);
  // Set points display status
  on_displayPointsCheckBox_toggled(false);

  // An attempt to generalize:
  //  paintButton(ui->backgroundColorPushButton,
  //  &ui->viewport->getBackgroundColor);
}

MainWindow::~MainWindow() { delete ui; }

/*!
 *  refactor to remove the code repetition
 *
 */
void MainWindow::on_backgroundColorPushButton_clicked() {
  QColor prev_color = ui->viewport->getBackgroundColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  ui->viewport->setBackgroundColor(color);
  ui->viewport->update();
  PaintBackgroundColorButton();
}
void MainWindow::on_lineColorPushButton_clicked() {
  QColor prev_color = ui->viewport->getLineColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  ui->viewport->setLineColor(color);
  ui->viewport->update();
  PaintLineColorButton();
}
void MainWindow::on_pointColorPushButton_clicked() {
  QColor prev_color = ui->viewport->getPointColor();
  QColor color = QColorDialog::getColor(prev_color, this);
  ui->viewport->setPointColor(color);
  ui->viewport->update();
  PaintPointColorButton();
}

void setLayoutWidgetsVisibility(const QLayout *layout, bool visibility) {
  for (int i = 0; i < layout->count(); ++i) {
    QWidget *widget = layout->itemAt(i)->widget();
    widget->setVisible(visibility);
  }
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

QString formColoredButtonStyleSheet(const QColor &c) {
  QString qss = QString(
                    "background-color: %1; border-width: 2px;"
                    "border-style: solid; border-radius: 5px;")
                    .arg(c.name());
  return qss;
}

::QColor convertColorToGreyscale(const QColor &c) {
  // Formula from the documentaiton:
  // https://doc.qt.io/archives/qt-4.8/qcolor.html#qGray
  int greyv = (c.red() * 11 + c.green() * 16 + c.blue() * 5) / 32;
  // Set the bounds
  static const int lbound = 43 * (float)255 / 100;
  static const int ubound = 81 * (float)255 / 100;
  if (greyv < lbound) {
    greyv = lbound;
  }
  if (greyv > ubound) {
    greyv = ubound;
  }

  QColor result = QColor(greyv, greyv, greyv);
  return result;
}

void MainWindow::on_displayLinesCheckBox_toggled(bool checked) {
  // Set the state in the viewport
  ui->viewport->setLineDisplayEnabled(checked);
  ui->viewport->update();
  // Set the checkbox check
  ui->displayLinesCheckBox->setChecked(checked);
  // Disable the settings' frame
  setLayoutWidgetsState(ui->lineSettingsLayout, checked);
  ui->lineStyleDashedCheckBox->setEnabled(checked);
  // Convert the button color to a greyscale
  static QColor color;
  color = ui->viewport->getLineColor();
  QColor result = color;
  if (checked == false) {
    result = ::convertColorToGreyscale(color);
  }
  QString qss = formColoredButtonStyleSheet(result);
  ui->lineColorPushButton->setStyleSheet(qss);
}

void MainWindow::on_displayPointsCheckBox_toggled(bool checked) {
  // Set the state in the viewport
  ui->viewport->setPointDisplayEnabled(checked);
  ui->viewport->update();
  // Set the checkbox check
  ui->displayPointsCheckBox->setChecked(checked);
  // Disable the settings' frame
  setLayoutWidgetsState(ui->pointSettingsLayout, checked);
  ui->pointStyleSquareCheckBox->setEnabled(checked);
  // Convert the button color to a greyscale if the checkbox is disabled
  static QColor color;
  color = ui->viewport->getPointColor();
  QColor result = color;
  if (checked == false) {
    result = ::convertColorToGreyscale(color);
  }
  QString qss = formColoredButtonStyleSheet(result);
  ui->pointColorPushButton->setStyleSheet(qss);
}

/*!
 *  refactor to remove the code repetition
 *
 */
void MainWindow::PaintLineColorButton() {
  QColor c = ui->viewport->getLineColor();
  QString qss = formColoredButtonStyleSheet(c);
  ui->lineColorPushButton->setStyleSheet(qss);
}
void MainWindow::PaintBackgroundColorButton() {
  QColor c = ui->viewport->getBackgroundColor();
  QString qss = formColoredButtonStyleSheet(c);
  ui->backgroundColorPushButton->setStyleSheet(qss);
}
void MainWindow::PaintPointColorButton() {
  QColor c = ui->viewport->getPointColor();
  QString qss = formColoredButtonStyleSheet(c);
  ui->pointColorPushButton->setStyleSheet(qss);
}
// void MainWindow::paintButton(QPushButton *b,
//                              QColor (OpenGLWidget::*getColor)()) {
//   QColor c = ui->viewport->getColor();
//   QString qss = formColoredButtonStyleSheet(c);
//   b->setStyleSheet(qss);
// }

void MainWindow::on_pointSizeSlider_valueChanged(int value) {
  ui->viewport->setPointSize(value);
  ui->viewport->update();
}

void MainWindow::on_pointStyleSquareCheckBox_toggled(bool checked) {
  if (checked == true)
    ui->viewport->setPointStyle(PointStyle::SQUARE);
  else
    ui->viewport->setPointStyle(PointStyle::CIRCLE);
  ui->viewport->update();
}

void MainWindow::on_lineWidthSlider_valueChanged(int value) {
  ui->viewport->setLineWidth(value);
  ui->viewport->update();
}

void MainWindow::on_lineStyleDashedCheckBox_toggled(bool checked) {
  if (checked == true)
    ui->viewport->setLineStyle(LineStyle::DASHED);
  else
    ui->viewport->setLineStyle(LineStyle::SOLID);
  ui->viewport->update();
}
