#include "mypanelopengl.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;
using cs40::Sphere;
using cs40::Square;
using cs40::Blob;
using cs40::Grid;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent)
    : QOpenGLWidget(parent), m_angles(0, 15., 0.) {

  for (int i = 0; i < 2; i++) {
    m_shaderPrograms[i] = NULL;
    m_vertexShaders[i] = NULL;
    m_fragmentShaders[i] = NULL;
  }

  m_sphere = NULL;
  m_square = NULL;
  m_grid = NULL;
  m_drawSphere = false;
  m_polymode = 2;
  m_cull = true;
  m_curr_prog = 0;

  m_depth = 25;
  m_thresh = .2;

  m_timer = NULL;
  m_dt = .1;
}

MyPanelOpenGL::~MyPanelOpenGL() {
  m_shaderPrograms[m_curr_prog]->release();
  delete m_sphere;
  m_sphere = NULL;
  delete m_square;
  m_square = NULL;
  destroyShaders(0);
  destroyShaders(1);
}

void MyPanelOpenGL::initializeGL() {

  std::cout << "\nKEY BINDINGS:" << std::endl;
  std::cout << "d: increase depth" << std::endl;
  std::cout << "D: decrease depth" << std::endl;
  std::cout << "t: increase threshold" << std::endl;
  std::cout << "T: decrease threshold" << std::endl;
  std::cout << "b: add blob" << std::endl;
  std::cout << "B: remove blob" << std::endl;
  std::cout << "s: draw spheres instead of metaballs" << std::endl;
  std::cout << "c: toggle culling" << std::endl;
  std::cout << "p: toggle polygon mode" << std::endl;
  std::cout << "l: change shader\n" << std::endl;


  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  updatePolyMode(m_polymode);

  glClearColor(0.9f, 0.9f, .9f, 0.0f);
  createShaders(0, "vshader.glsl", "fshader.glsl");
  createShaders(1, "vfraglight.glsl", "ffraglight.glsl");

  m_sphere = new Sphere(1, 20, 20);
  m_sphere->setColor(vec3(0, 0, 1));
  //m_square = new Square(1.);

  int nblob = 6;
  makeBlobs(nblob);

  m_grid = new Grid(50,50,m_depth,&m_blobs,m_thresh);

  m_projection.perspective(40, 1, 1, 8);
  m_camera.lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1., 0.));
  m_model.setToIdentity();

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  m_timer->start(10);
}

void MyPanelOpenGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyPanelOpenGL::paintGL() {
  /* clear both color and depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updatePolyMode(m_polymode);
  if (m_cull) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }

  if (!m_shaderPrograms[m_curr_prog]) {
    return;
  }
  m_shaderPrograms[m_curr_prog]->bind();
  mat4 mview = m_camera * m_model;
  m_shaderPrograms[m_curr_prog]->setUniformValue("projection", m_projection);
  m_shaderPrograms[m_curr_prog]->setUniformValue("camera", m_camera);
  m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
  m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",
                                                 mview.normalMatrix());
  m_shaderPrograms[m_curr_prog]->setUniformValue(
      "lightPos", vec4(1.0, 0, 2, 1.)); // in world coordinates

  if(m_drawSphere){
    for(int i=0; i<m_blobs.size(); i++){
      m_modelStack.push();
      m_modelStack.translate(m_blobs[i]->getPos());
      m_modelStack.scale(m_blobs[i]->getRad(),m_blobs[i]->getRad(),m_blobs[i]->getRad());

      m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_modelStack.top());
      m_sphere->draw(m_shaderPrograms[m_curr_prog]);
      m_modelStack.pop();
    }
  }
  else{
    m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_modelStack.top());
    m_grid->draw(m_shaderPrograms[m_curr_prog]);
  }

  glFlush();
}

void MyPanelOpenGL::makeBlobs(int numBlobs){
  float x, y, vx, vy, r;
  float maxV = 0.5;
  for (int i=0; i<numBlobs; i++) {

      x = rand() / (float)RAND_MAX * 2 - 1;
      y = rand() / (float)RAND_MAX * 2 - 1;
      vx = rand() / (float)RAND_MAX * maxV * 2 - maxV;
      vy = rand() / (float)RAND_MAX * maxV * 2 - maxV;
      r = 2*m_thresh;
      m_blobs.append(new Blob(r, vec3(x,y,0), vec3(vx, vy,0)));
  }
}

void MyPanelOpenGL::updateTime(){
    /* trigger a refresh */
    for (int i=0; i < m_blobs.size(); i++) {
        m_blobs.at(i)->move(m_dt);
    }
    update();
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *event) {
  int step = 3;
  /*Enable strong Focus on GL Widget to process key events*/
  switch (event->key()) {
  case Qt::Key_D:
    if (event->text() == "d") {
        m_depth += step;
        if (m_depth > 100) {m_depth = 100;}
    } else {
        m_depth -= step;
        if (m_depth < 1) {m_depth = 1;}
    }
    m_grid->setDepth(m_depth);
    std::cout << "depth is: " << m_depth << std::endl;
    break;
  case Qt::Key_T:
    if (event->text() == "t") {
        m_thresh += .01;
        if (m_thresh > 1.0) {m_thresh = 1;}
    } else {
        m_thresh -= .01;
        if (m_thresh <= 0) {m_thresh = 0;}
    }
    std::cout << "threshold is: " << m_thresh << std::endl;
    //set radius based on new threshold so that blobs stay in window
    for(int i=0; i<m_blobs.size(); i++){
      m_blobs[i]->setRad(m_thresh*2);
    }
    m_grid->setThreshold(m_thresh);
    break;
  case Qt::Key_B:
    if (event->text() == "b") {
        makeBlobs(1);
    } else {
        if (m_blobs.size() > 0){
          m_blobs.pop_front();
        }
    }
    std::cout << "there are " << m_blobs.size() << " blobs" << std::endl;
    break;
  case Qt::Key_S:
    m_drawSphere = !m_drawSphere;
    if(m_drawSphere){
      m_dt /= 3.0;
    }
    else{
      m_dt *= 3.0;
    }
    break;
  case Qt::Key_C:
    m_cull = !m_cull;
    break;
  case Qt::Key_P:
    m_polymode = (m_polymode + 1) % 3;
    break;
  case Qt::Key_L:
    m_curr_prog = (m_curr_prog + 1) % 2;
    break;
  default:
    QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt) {
  if (idx == 0) {
    m_angles.setX(m_angles.x() + amt);
  } else if (idx == 1) {
    m_angles.setY(m_angles.y() + amt);
  } else if (idx == 2) {
    m_angles.setZ(m_angles.z() + amt);
  }
}

qreal MyPanelOpenGL::wrap(qreal amt) {
  if (amt > 360.) {
    return amt - 360.;
  } else if (amt < 0.) {
    return amt + 360.;
  }
  return amt;
}

void MyPanelOpenGL::updatePolyMode(int val) {
  GLenum mode = GL_NONE;
  if (val == 0) {
    mode = GL_POINT;
  } else if (val == 1) {
    mode = GL_LINE;
  } else if (val == 2) {
    mode = GL_FILL;
  }

  if (mode != GL_NONE) {
    glPolygonMode(GL_FRONT_AND_BACK, mode);
  }
  // glPolygonMode(GL_BACK,GL_POINT);
}

void MyPanelOpenGL::createShaders(int i, QString vertName, QString fragName) {

  //cout << "building shader " << i << endl;
  destroyShaders(i); // get rid of any old shaders
  m_vertexShaders[i] = new QOpenGLShader(QOpenGLShader::Vertex);
  if (!m_vertexShaders[i]->compileSourceFile(vertName)) {
    qWarning() << m_vertexShaders[i]->log();
  }

  m_fragmentShaders[i] = new QOpenGLShader(QOpenGLShader::Fragment);
  if (!m_fragmentShaders[i]->compileSourceFile(fragName)) {
    qWarning() << m_fragmentShaders[i]->log();
  }

  m_shaderPrograms[i] = new QOpenGLShaderProgram();
  m_shaderPrograms[i]->addShader(m_vertexShaders[i]);
  m_shaderPrograms[i]->addShader(m_fragmentShaders[i]);

  if (!m_shaderPrograms[i]->link()) {
    qWarning() << m_shaderPrograms[i]->log() << endl;
  }
}

void MyPanelOpenGL::destroyShaders(int i) {
  delete m_vertexShaders[i];
  m_vertexShaders[i] = NULL;
  delete m_fragmentShaders[i];
  m_fragmentShaders[i] = NULL;

  if (m_shaderPrograms[i]) {
    m_shaderPrograms[i]->release();
    delete m_shaderPrograms[i];
    m_shaderPrograms[i] = NULL;
  }
}
