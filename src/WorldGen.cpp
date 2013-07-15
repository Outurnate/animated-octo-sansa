#include "WorldGen.hpp"
#include "WorldPreviewWidget.hpp"
#include "WorldPreview.hpp"

WorldGen::WorldGen(QWidget *parent = 0)
  : QDialog(parent), ui(new Ui::WorldGen())
{
  ui->setupUi(this);
  WorldPreviewWidget *wprev = new WorldPreviewWidget(new WorldPreview());
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
