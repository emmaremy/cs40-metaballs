#pragma once

#include <QtOpenGL>
#include <cmath>
#include "blob.h"

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40 {

class Grid {
public:

  Grid(int w, int h, int d, QList<cs40::Blob*>* blobs, double t);

  ~Grid();

  /* draw the grid using provided shader program  */
  void draw(QOpenGLShaderProgram *prog);

  /* Set Ambient and Diffuse color of Grid */
  inline void setColor(const vec3 &color) {
    m_color = color;
    m_color.setW(1.);
  }

  void setThreshold(float t){m_threshold = m_width * t;}
  void setDepth(float d){m_depth = d;}

  inline void setSpecularColor(const vec3 &color) {
    m_spec_color = color;
    m_spec_color.setW(1.);
  }

  /* Get current color of object */
  inline vec4 getAmbientAndDiffuseColor() const { return m_color; }

  inline vec4 getSpecularColor() const { return m_spec_color; }

private:
  /* Generate Vertex Buffer Objects, but do not bind/allocate.
   * Return true if generate was a success, false otherwise
   */
  bool initVBO();

protected:
  vec4 m_color;
  vec4 m_spec_color;
  QOpenGLBuffer *m_vbo;
  QOpenGLVertexArrayObject *m_vao;
  int m_width;
  int m_height;
  int m_depth;
  int m_ncells;
  int m_ntriangles;
  int m_nblobs;
  QList<cs40::Blob*>* m_blobs;
  double m_threshold;
  vec3 *triangles;
  vec3 *normals;
};

} // namespace
