#include "mainwindow.hpp"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QShortcut>

#include "./ui_mainwindow.h"
#include "gifimage/qgifimage.h"

enum ControlSteps {
  WIDTH = 200,
  LOCATION = 1000,
  ROTATION = 1800,
  SCALE = 1000,
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
  on_perspectiveProjectionButton_toggled(true);
  /* Set up primitives' size controls */
  setupWidthControls(ui->lineWidthSlider, ui->lineWidthSpinBox);
  setupWidthControls(ui->pointSizeSlider, ui->pointSizeSpinBox);
  /* Set up location controls */
  setupLocationControls(ui->xLocationSlider, ui->xLocationSpinbox);
  setupLocationControls(ui->yLocationSlider, ui->yLocationSpinbox);
  setupLocationControls(ui->zLocationSlider, ui->zLocationSpinbox);
  /* Set up rotation controls */
  setupRotationControls(ui->xRotationSlider, ui->xRotationSpinbox);
  setupRotationControls(ui->yRotationSlider, ui->yRotationSpinbox);
  setupRotationControls(ui->zRotationSlider, ui->zRotationSpinbox);
  /* Set up scale controls */
  setupScaleControls(ui->xScaleSlider, ui->xScaleSpinbox);
  setupScaleControls(ui->yScaleSlider, ui->yScaleSpinbox);
  setupScaleControls(ui->zScaleSlider, ui->zScaleSpinbox);
  setupScaleControls(ui->uScaleSlider, ui->uScaleSpinbox);
  /* Set up shortcuts */
  new QShortcut(QKeySequence(tr("Ctrl+O")), this, SLOT(openFile()));
  new QShortcut(QKeySequence(tr("L")), ui->xLocationSpinbox, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("R")), ui->xRotationSpinbox, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("S")), ui->xScaleSpinbox, SLOT(setFocus()));
  new QShortcut(QKeySequence(tr("E")), ui->displayLinesCheckBox,
                SLOT(toggle()));
  new QShortcut(QKeySequence(tr("V")), ui->displayPointsCheckBox,
                SLOT(toggle()));
  connect(&gif_timer, SIGNAL(timeout()), this, SLOT(RecordGifFrame()));
  resetSettings();
  loadSettings();
}

MainWindow::~MainWindow() {
  this->saveSettings(nullptr);
  delete ui;
}

void MainWindow::saveSettings(QString file_name) {
  QSettings settings("settings.conf", QSettings::IniFormat);
  if (file_name != nullptr) {
    settings.beginGroup("Filename");
    settings.setValue("model", file_name);
    settings.endGroup();
  }
  settings.beginGroup("MainWindow");
  settings.setValue("geometry", saveGeometry());
  settings.endGroup();
  settings.beginGroup("DrawSettings");
  settings.setValue("BackgroundColor", ui->viewport->getBackgroundColor());
  settings.setValue("DrawLines", ui->displayLinesCheckBox->checkState());
  settings.setValue("DashedLines", ui->lineStyleDashedCheckBox->checkState());
  settings.setValue("LinesColor", ui->viewport->getLineColor());
  settings.setValue("LinesWidth", ui->lineWidthSpinBox->value());
  settings.setValue("DrawPoints", ui->displayPointsCheckBox->checkState());
  settings.setValue("PointsType", ui->pointStyleSquareCheckBox->checkState());
  settings.setValue("PointsColor", ui->viewport->getPointColor());
  settings.setValue("PointsSize", ui->pointSizeSpinBox->value());
  settings.endGroup();
  settings.beginGroup("CameraProjection");
  settings.setValue("Perspective", ui->orthographicProjButton->isChecked());
  settings.setValue("Ortho", ui->perspectiveProjectionButton->isChecked());
  settings.endGroup();
  settings.beginGroup("Location");
  settings.setValue("xLocationSpinbox", ui->xLocationSpinbox->value());
  settings.setValue("yLocationSpinbox", ui->yLocationSpinbox->value());
  settings.setValue("zLocationSpinbox", ui->zLocationSpinbox->value());
  settings.endGroup();
  settings.beginGroup("Rotation");
  settings.setValue("xRotationSpinbox", ui->xRotationSpinbox->value());
  settings.setValue("yRotationSpinbox", ui->yRotationSpinbox->value());
  settings.setValue("zRotationSpinbox", ui->zRotationSpinbox->value());
  settings.endGroup();
  settings.beginGroup("Scale");
  settings.setValue("xScaleSpinbox", ui->xScaleSpinbox->value());
  settings.setValue("yScaleSpinbox", ui->yScaleSpinbox->value());
  settings.setValue("zScaleSpinbox", ui->zScaleSpinbox->value());
  settings.setValue("uScaleSpinbox", ui->uScaleSpinbox->value());
  settings.endGroup();
}

void MainWindow::loadSettings() {
  if (QFile("settings.conf").exists()) {
    QSettings settings("settings.conf", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    if (settings.contains("geometry")) {
      restoreGeometry(settings.value("geometry").toByteArray());
    }
    settings.endGroup();
    settings.beginGroup("DrawSettings");
    if (settings.contains("BackgroundColor")) {
      ui->viewport->setBackgroundColor(
          settings.value("BackgroundColor").value<QColor>());
    }
    if (settings.contains("DrawLines")) {
      ui->displayLinesCheckBox->setCheckState(
          settings.value("DrawLines").value<Qt::CheckState>());
    }
    if (settings.contains("DashedLines")) {
      ui->lineStyleDashedCheckBox->setCheckState(
          settings.value("DashedLines").value<Qt::CheckState>());
    }
    if (settings.contains("LinesColor")) {
      ui->viewport->setLineColor(settings.value("LinesColor").value<QColor>());
    }
    if (settings.contains("LinesWidth")) {
      ui->lineWidthSpinBox->setValue(settings.value("LinesWidth").toDouble());
    }
    if (settings.contains("DrawPoints")) {
      ui->displayPointsCheckBox->setCheckState(
          settings.value("DrawPoints").value<Qt::CheckState>());
    }
    if (settings.contains("PointsType")) {
      ui->pointStyleSquareCheckBox->setCheckState(
          settings.value("PointsType").value<Qt::CheckState>());
    }
    if (settings.contains("PointsColor")) {
      ui->viewport->setPointColor(
          settings.value("PointsColor").value<QColor>());
    }
    if (settings.contains("PointsSize")) {
      ui->pointSizeSpinBox->setValue(settings.value("PointsSize").toDouble());
    }
    settings.endGroup();
    settings.beginGroup("CameraProjection");
    if (settings.contains("Perspective")) {
      if (settings.value("Perspective").toBool()) {
        ui->orthographicProjectionButton->setChecked(1);
      }
    }
    if (settings.contains("Ortho")) {
      if (settings.value("Ortho").toBool()) {
        ui->perspectiveProjectionButton->setChecked(1);
      }
    }
    settings.endGroup();
    settings.beginGroup("Location");
    if (settings.contains("xLocationSpinbox")) {
      ui->xLocationSpinbox->setValue(
          settings.value("xLocationSpinbox").toDouble());
    }
    if (settings.contains("yLocationSpinbox")) {
      ui->yLocationSpinbox->setValue(
          settings.value("yLocationSpinbox").toDouble());
    }
    if (settings.contains("zLocationSpinbox")) {
      ui->zLocationSpinbox->setValue(
          settings.value("zLocationSpinbox").toDouble());
    }
    settings.endGroup();
    settings.beginGroup("Rotation");
    if (settings.contains("xRotationSpinbox")) {
      ui->xRotationSpinbox->setValue(
          settings.value("xRotationSpinbox").toDouble());
    }
    if (settings.contains("yRotationSpinbox")) {
      ui->yRotationSpinbox->setValue(
          settings.value("yRotationSpinbox").toDouble());
    }
    if (settings.contains("zRotationSpinbox")) {
      ui->zRotationSpinbox->setValue(
          settings.value("zRotationSpinbox").toDouble());
    }
    settings.endGroup();
    settings.beginGroup("Scale");
    if (settings.contains("xScaleSpinbox")) {
      ui->xScaleSpinbox->setValue(settings.value("xScaleSpinbox").toDouble());
    }
    if (settings.contains("yScaleSpinbox")) {
      ui->yScaleSpinbox->setValue(settings.value("yScaleSpinbox").toDouble());
    }
    if (settings.contains("zScaleSpinbox")) {
      ui->zScaleSpinbox->setValue(settings.value("zScaleSpinbox").toDouble());
    }
    if (settings.contains("uScaleSpinbox")) {
      ui->uScaleSpinbox->setValue(settings.value("uScaleSpinbox").toDouble());
    }
    settings.endGroup();

    settings.beginGroup("Filename");
    if (settings.contains("model")) {
      ui->viewport->setFileName(
          settings.value("model").toString().toStdString());
      ui->viewport->loadModel();
    }
    settings.endGroup();
  }
}

void MainWindow::resetSettings() {
  on_lineWidthSlider_doubleValueChanged(1.0);
  ui->lineWidthSpinBox->setValue(1.0);

  on_pointSizeSlider_doubleValueChanged(1.0);
  ui->pointSizeSpinBox->setValue(1.0);

  ui->xLocationSlider->setValue(0);
  ui->yLocationSlider->setValue(0);
  ui->zLocationSlider->setValue(0);

  ui->xRotationSlider->setValue(0);
  ui->yRotationSlider->setValue(0);
  ui->zRotationSlider->setValue(0);

  ui->xScaleSpinbox->setValue(1);
  ui->yScaleSpinbox->setValue(1);
  ui->zScaleSpinbox->setValue(1);
  ui->uScaleSpinbox->setValue(1);
}

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

void syncSliderWithSpinbox(DoubleSlider *s, QDoubleSpinBox *sb) {
  QObject::connect(s, &DoubleSlider::doubleValueChanged, sb,
                   &QDoubleSpinBox::setValue);
  QObject::connect(sb, &QDoubleSpinBox::valueChanged, s,
                   &DoubleSlider::setDoubleValue);
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

void MainWindow::on_orthographicProjectionButton_toggled(bool checked) {
  ui->viewport->setProjectionType(ProjectionType::ORTHOGONAL);
  ui->viewport->update();
}
void MainWindow::on_perspectiveProjectionButton_toggled(bool checked) {
  ui->viewport->setProjectionType(ProjectionType::PERSPECTIVE);
  ui->viewport->update();
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

/* Primitives' size related functions */

void MainWindow::setupWidthControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSpinbox(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::WIDTH;
  const float sb_limit = 20.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(1);
  sb->setMinimum(1.0);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(10);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
}
void MainWindow::on_lineWidthSlider_doubleValueChanged(double value) {
  ui->viewport->setLineWidth(value);
  ui->viewport->update();
}
void MainWindow::on_pointSizeSlider_doubleValueChanged(double value) {
  ui->viewport->setPointSize(value);
  ui->viewport->update();
}

/* Location related functions */

void MainWindow::setupLocationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSpinbox(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::LOCATION;
  const float sb_limit = 1.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(3);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(-steps_count);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count;
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

/* Rotation related functions */

void MainWindow::setupRotationControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSpinbox(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::ROTATION;
  const float sb_limit = 180.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(1);
  sb->setMinimum(-sb_limit);
  sb->setMaximum(+sb_limit);
  /* Set up the slider */
  s->setMinimum(-steps_count);
  s->setMaximum(+steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
}
void MainWindow::on_xRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationX(value);
  ui->viewport->update();
}
void MainWindow::on_yRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationY(value);
  ui->viewport->update();
}
void MainWindow::on_zRotationSlider_doubleValueChanged(double value) {
  ui->viewport->setRotationZ(value);
  ui->viewport->update();
}
void MainWindow::on_rotationResetPushButton_clicked() {
  ui->xRotationSlider->setValue(0);
  ui->yRotationSlider->setValue(0);
  ui->zRotationSlider->setValue(0);
}

/* Scale related functions */

void MainWindow::setupScaleControls(DoubleSlider *s, QDoubleSpinBox *sb) {
  syncSliderWithSpinbox(s, sb);
  /* Set up the spinbox */
  const unsigned int steps_count = ControlSteps::SCALE;
  const float sb_limit = 10.0f;
  const float sb_step = sb_limit / steps_count;
  sb->setSingleStep(sb_step);
  sb->setDecimals(2);
  sb->setMinimum(0.01f);
  sb->setMaximum(10.00f);
  /* Set up the slider */
  s->setMinimum(1);
  s->setMaximum(steps_count);
  // internally the slider is of int type but emits the signal of type double
  s->divisor = steps_count / sb_limit;
  s->setDoubleValue(1.0f);
}
void MainWindow::on_xScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleX(value);
  ui->viewport->update();
}
void MainWindow::on_yScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleY(value);
  ui->viewport->update();
}
void MainWindow::on_zScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleZ(value);
  ui->viewport->update();
}
void MainWindow::on_uScaleSlider_doubleValueChanged(double value) {
  ui->viewport->setScaleU(value);
  ui->viewport->update();
}
void MainWindow::on_scaleResetPushButton_clicked() {
  ui->xScaleSpinbox->setValue(1);
  ui->yScaleSpinbox->setValue(1);
  ui->zScaleSpinbox->setValue(1);
  ui->uScaleSpinbox->setValue(1);
}

void MainWindow::on_openFilePushButton_released() { MainWindow::openFile(); }
void MainWindow::openFile() {
  QString dir = QDir::homePath() + "/Downloads/3Dmodels";
  QString file_name = QFileDialog::getOpenFileName(
      this, "Open 3d model", dir, "geometry definition file (*.obj)");
  if (file_name.isEmpty()) {
    return;
  }

  this->saveSettings(file_name);
  ui->viewport->setFileName(file_name.toStdString());
  ui->viewport->loadModel();
}

void MainWindow::on_screenshotPushButton_released() {
  QImage screenshot = ui->viewport->grabFramebuffer();
  QString dir = QDir::homePath();
  QString extension = "*.jpeg;;*.bmp";
  QString file_name = QFileDialog::getSaveFileName(this, tr("Save screenshot"),
                                                   dir, extension, &extension);
  if (file_name.isEmpty()) {
    return;
  }

  extension.remove(0, 2);
  if (file_name.right(extension.length() + 1) != ("." + extension)) {
    file_name = file_name + "." + extension;
  }
  screenshot.save(file_name, extension.toStdString().c_str(), 80);
}

void MainWindow::on_gifCapturePushButton_released() {
  gif = new QGifImage();
  gif_timer.start(1000 / gif_fps);
}
void MainWindow::RecordGifFrame() {
  if (gif_frame_counter < gif_fps * gif_sec) {
    gif_frame_counter++;
    gif->setDefaultDelay(1000 / gif_fps);
    QImage frame = ui->viewport->grabFramebuffer().scaled(gif_size);
    gif->addFrame(frame);
  } else {
    gif_timer.stop();
    gif_frame_counter = 0;
    QString dir = QDir::homePath();
    QString file_name = QFileDialog::getSaveFileName(this, "Save Gif", dir,
                                                     "Gif files (*.gif)");
    if (!file_name.isEmpty()) {
      gif->save(file_name);
    }
    delete gif;
  }
}
