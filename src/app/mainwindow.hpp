#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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

 private slots:
  void on_displayVerticesCheckBox_stateChanged(int arg1);
  void on_displayLinesCheckBox_stateChanged(int arg1);
  void on_backgroundColorPushButton_clicked();
  void on_linesColorPushButton_clicked();
  void on_pointColorPushButton_clicked();
  void on_displayLinesCheckBox_toggled(bool checked);
  void on_displayVerticesCheckBox_toggled(bool checked);

  void PaintLineColorButton();
  void PaintBackgroundColorButton();
  void PaintVertexColorButton();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_HPP
