#pragma once

#include <QString>
#include <QtOpenGL>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40{

class Blob
{
public:
    Blob(float r, vec3 c, vec3 velocity);

    ~Blob(){ }

    /* move the BLOBs. */
    void move(float dt);

    vec3 getPos(){ return m_center; }
    float getRad(){ return m_radius; }
    void setRad(float r){m_radius = r;}

private:
    Blob();
    float m_radius;
    vec3 m_center;
    vec3 m_vel;

};

} //namespace
