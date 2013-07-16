#include "WorldGen.hpp"
#include "WorldPreviewWidget.hpp"
#include "WorldPreviewScene.hpp"

WorldGen::WorldGen(QWidget *parent)
  : QDialog(parent), ui(new Ui::WorldGen())
{
  ui->setupUi(this);
  WorldPreviewWidget *wprev = new WorldPreviewWidget(new WorldPreviewScene());
  QGridLayout *grid = new QGridLayout();
  grid->addWidget(wprev);
  ui->glContainer->setLayout(grid);
}

WorldGen::~WorldGen()
{
  delete ui;
}

void WorldGen::ProduceMap()
{
}
