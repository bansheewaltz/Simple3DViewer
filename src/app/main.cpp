#include <QApplication>

#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Style settings
  //  a.setStyle("Windows");
  //  a.setStyle("Fusion");
  QPalette pal = QPalette();
  pal.setColor(QPalette::Active, QPalette::WindowText, (QColor) "#D0D0D0");
  qApp->setPalette(pal);

  MainWindow w;
  w.show();

  return a.exec();
}
