#include "grid.h"
#include "marchingcubes.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Grid::Grid(int w, int h, int d, QList<cs40::Blob*>* blobs, double t) {

  m_color = vec4(0, 0, 1, 1);
  m_spec_color = vec4(1, 1, 1, 1);
  m_vbo = NULL;
  m_width = w;
  m_height = h;
  m_depth = d;
  m_ncells = m_width * m_height * m_depth;
  m_ntriangles = 0;
  setThreshold(t);
  m_blobs = blobs;

  triangles = new vec3[m_ncells*3*5];
  normals = new vec3[m_ncells*3*5];

}

bool Grid::initVBO() {
  m_vao = new QOpenGLVertexArrayObject();
  bool ok = m_vao->create();
  if (!ok) {
    return false;
  }
  m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  ok = m_vbo->create();
  if (!ok) {
    return false;
  }
  m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
  return ok;
}

Grid::~Grid() {
  delete[] triangles;
  triangles = NULL;
  delete[] normals;
  normals = NULL;

  if (m_vbo) {
    m_vbo->release();
    delete m_vbo;
    m_vbo = NULL;
  }
  if (m_vao) {
    m_vao->release();
    delete m_vao;
    m_vao = NULL;
  }
}

void Grid::draw(QOpenGLShaderProgram *prog) {
  if (!prog) {
    return;
  }

  //loop through all the cells and find the triangles
  m_ntriangles = 0;
  GRIDCELL cell;
  TRIANGLE celltriangles[5];
  int ncelltriangles;
  bool nearBlob;

  for(int i=0; i<m_width; i++){
    for(int j=0; j<m_height; j++){
      for(int k=0; k<m_depth; k++){
        nearBlob = false;

        //initialize cell (.p has vertex coords and .val has min distances)
        cell.p[0] = vec3(i,j,k);
        cell.p[1] = vec3(i+1,j,k);
        cell.p[2] = vec3(i+1,j,k+1);
        cell.p[3] = vec3(i,j,k+1);
        cell.p[4] = vec3(i,j+1,k);
        cell.p[5] = vec3(i+1,j+1,k);
        cell.p[6] = vec3(i+1,j+1,k+1);
        cell.p[7] = vec3(i,j+1,k+1);

        //loop through cell vertices and fill in .val
        vec3 bpos;
        for(int l=0; l<8; l++){
          //initialize mindist array with dist to first blob
          bpos = m_blobs->at(0)->getPos()*vec3(m_width/2, m_height/2, m_depth/2)+vec3(m_width/2, m_height/2, m_depth/2);
          cell.val[l] = (cell.p[l]-bpos).length();

          //find dist to center of closest blob //TODO account for radius
          for(int q=0; q<m_blobs->size(); q++){
            bpos = m_blobs->at(q)->getPos()*vec3(m_width/2, m_height/2, m_depth/2)+vec3(m_width/2, m_height/2, m_depth/2);
            float dist = (cell.p[l]-bpos).length();
            if (dist < cell.val[l]){
              cell.val[l] = dist;
            }
          }
          if (cell.val[l] < m_threshold){
            nearBlob = true;
          }
        }

        //no need to try to find triangles if no vertex is near a blob
        if (!nearBlob){
          continue;
        }

        ncelltriangles = cs40::polygonise(cell, m_threshold, celltriangles);

        for(int l=0; l<ncelltriangles; l++){
          triangles[3*m_ntriangles] = celltriangles[l].p[0];
          triangles[3*m_ntriangles+1] = celltriangles[l].p[1];
          triangles[3*m_ntriangles+2] = celltriangles[l].p[2];
          m_ntriangles++;
        }
      }
    }
  } //end loop over cells

  //scale from [0,m_width]x[0,m_height]x[0,m_depth] to [-1,1]x[-1,1]x[-1,1]
  for(int i=0; i<m_ntriangles*3; i++){
    triangles[i] = vec3(triangles[i].x()*2.0/m_width-1, triangles[i].y()*2.0/m_height-1, triangles[i].z()*2.0/m_depth-1);

  }

  for(int i=0; i<m_ntriangles*3; i += 3){
    vec3 norm = vec3::crossProduct((triangles[i+1]-triangles[i]), (triangles[i+2]-triangles[i]));
    norm.normalize();
    normals[i] = norm;
    normals[i+1] = normals[i];
    normals[i+2] = normals[i];
  }

  //put triangles in VBO
  if (initVBO()) {
    int dataSize = m_ntriangles * 3 * sizeof(vec3); //3 for 3 pts per triangle

    m_vbo->bind();
    m_vbo->allocate(3*dataSize);
    m_vbo->write(0, triangles, dataSize);
    m_vbo->write(dataSize, normals, dataSize);
    m_vbo->release();
  }

  m_vao->bind();
  m_vbo->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  prog->enableAttributeArray("vPosition");
  prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 3, 0);

  prog->enableAttributeArray("vNormal");
  prog->setAttributeBuffer("vNormal", GL_FLOAT,
                           m_ntriangles * 3 * sizeof(vec3), 3, 0);

  glDrawArrays(GL_TRIANGLES, 0, m_ntriangles*3);
  m_vbo->release();
  m_vao->release();
}
