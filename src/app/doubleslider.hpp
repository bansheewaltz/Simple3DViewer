#include "QtCore/qtmetamacros.h"
#include "QtWidgets/qslider.h"

class DoubleSlider : public QSlider {
  Q_OBJECT

 public:
  int divisor;

 public:
  DoubleSlider(QWidget *parent = 0);
  void setDoubleValue(double value);

 signals:
  void doubleValueChanged(double value);

 public slots:
  void notifyValueChanged(int value);
};
