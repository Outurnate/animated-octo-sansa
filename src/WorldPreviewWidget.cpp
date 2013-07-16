#include "WorldPreviewWidget.hpp"

WorldPreviewWidget::WorldPreviewWidget(Scene *previewScene, QWidget *parent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), previewScene(previewScene)
{
}

WorldPreviewWidget::~WorldPreviewWidget()
{
  previewScene->destroy();
}

QSize WorldPreviewWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize WorldPreviewWidget::sizeHint() const
{
  return QSize(400, 400);
}

void WorldPreviewWidget::setMetaballs(unsigned num)
{
  if (numMetaballs != num)
  {
    numMetaballs = num;
    emit metaballsChanged(num);
    updateGL();
  }
}

void WorldPreviewWidget::initializeGL()
{
  qglClearColor(QColor::fromRgb(0.0, 0.0, 0.0));

  previewScene->init();
}

void WorldPreviewWidget::resizeGL(int width, int height)
{
  this->width = width;
  this->height = height;
}

void WorldPreviewWidget::paintGL()
{
  previewScene->render(0, width, height);
}
