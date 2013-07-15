#ifndef WORLDGEN_H
#define WORLDGEN_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "ui_worldgen.h"
#pragma GCC diagnostic pop

namespace Ui
{
  class WorldGen;
}

class WorldGen : public QDialog
{
  Q_OBJECT
public:
  explicit WorldGen(QWidget *parent = 0);
  ~WorldGen();

  void ProduceMap();
private:
  Ui::WorldGen *ui;
};

#endif
