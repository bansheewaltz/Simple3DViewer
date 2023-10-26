#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>

#include "./ui_mainwindow.h"

// void changeLabelPalette(QWidget *l);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Line drawing
  ui->displayPointsCheckBox->setCheckState(Qt::CheckState::Checked);
  ui->viewport->is_line_drawing_active = true;
  on_displayLinesCheckBox_toggled(true);
  // Point drawing
  ui->displayPointsCheckBox->setCheckState(Qt::CheckState::Unchecked);
  ui->viewport->is_point_drawing_active = false;
  on_displayPointsCheckBox_toggled(false);
  //
  //  paintButton(ui->backgroundColorPushButton,
  //  &ui->viewport->getBackgroundColor);
  PaintLineColorButton();
  PaintBackgroundColorButton();
  PaintPointColorButton();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_displayPointsCheckBox_stateChanged(int arg1) {
  bool state = ui->viewport->is_point_drawing_active;
  ui->viewport->is_point_drawing_active = !state;
  ui->viewport->update();
}

void MainWindow::on_displayLinesCheckBox_stateChanged(int arg1) {
  bool state = ui->viewport->is_line_drawing_active;
  ui->viewport->is_line_drawing_active = !state;
  ui->viewport->update();
}

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
  // Set the lower bound
  const int bound = (float)255 / 100 * 44;
  if (greyv < bound) {
    greyv = bound;
  }

  QColor result = QColor(greyv, greyv, greyv);
  return result;
}

void MainWindow::on_displayLinesCheckBox_toggled(bool checked) {
  setLayoutWidgetsState(ui->lineSettingsLayout, checked);
  ui->lineStyleDashedCheckBox->setEnabled(checked);
  //  setLayoutWidgetsVisibility(ui->lineSettingsLayout, checked);

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
  setLayoutWidgetsState(ui->pointSettingsLayout, checked);

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
