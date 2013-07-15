#include "WorldGen.hpp"

WorldGen::WorldGen(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WorldGen())
{
  ui->setupUi(this);
}

WorldGen::~WorldGen()
{
  delete ui;
}

void WorldGen::ProduceMap()
{
}
