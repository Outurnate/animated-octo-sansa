#ifndef WORLDPREVIEWWIDGET_H
#define WORLDPREVIEWWIDGET_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Weffc++"
#include <QGLWidget>
#pragma GCC diagnostic pop

#include "Scene.hpp"

class WorldPreviewWidget : public QGLWidget
{
  Q_OBJECT
private:
  unsigned numMetaballs;
  int width, height;
  Scene *previewScene;
public:
  WorldPreviewWidget(Scene *previewScene, QWidget *parent = 0);
  ~WorldPreviewWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
public slots:
  void setMetaballs(unsigned num);
signals:
  void metaballsChanged(unsigned num);
protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
};

#endif
