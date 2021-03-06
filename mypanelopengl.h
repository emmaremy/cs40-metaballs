#pragma once

#include "common/sphere.h"
#include "common/square.h"
#include "common/blob.h"
#include "common/matrixstack.h"
#include "common/marchingcubes.h"
#include "common/grid.h"
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QtWidgets>

class MyPanelOpenGL : public QOpenGLWidget {
  typedef QVector4D point4;
  typedef QVector4D color4;
  typedef QVector3D vec3;
  typedef QMatrix4x4 mat4;

  Q_OBJECT

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void keyPressEvent(QKeyEvent *event);

public:
  explicit MyPanelOpenGL(QWidget *parent = 0);
  virtual ~MyPanelOpenGL();

private:
  /* simple test shapes */
  cs40::Sphere *m_sphere;
  cs40::Square *m_square;

  QList<cs40::Blob*> m_blobs;
  cs40::Grid *m_grid;

  int m_polymode;
  bool m_drawSphere;
  bool m_cull;

  int m_depth;
  double m_thresh;

  vec3 m_angles; /* Euler angles for rotation */

  mat4 m_model;
  mat4 m_camera;
  mat4 m_projection;
  cs40::MatrixStack m_modelStack;

  /* set this up when you are ready to animate */
  QTimer* m_timer;      /* event timer */
  float m_dt;

  /* Shaders and program */
  QOpenGLShader *m_vertexShaders[2];
  QOpenGLShader *m_fragmentShaders[2];
  QOpenGLShaderProgram *m_shaderPrograms[2];
  int m_curr_prog; // current program ID

  void makeBlobs(int numBlobs);

  /* draw square originally in z=0 plane, rotated about y-axis
   * by yangle (in degrees) */
  void drawSquare(float yangle);

  /* update Euler angle at index idx by amt
   * idx=0,1,2 -> x,y,z */
  void updateAngles(int idx, qreal amt);

  /* wrap a angle value to the range 0..360*/
  qreal wrap(qreal amt);

  /* update Polygon draw mode based
   * 0 : point
   * 1 : line
   * 2 : polygon */
  void updatePolyMode(int val);

  void createShaders(int i, QString vertName, QString fragName);
  void destroyShaders(int i);

signals:

public slots:
    /* trigger update of GL window */
    void updateTime();
};
