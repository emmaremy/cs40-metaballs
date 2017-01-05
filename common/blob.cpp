#include "blob.h"

using namespace cs40;

Blob::Blob(float r, vec3 c, vec3 velocity){
   m_radius = r;
   m_center = c;
   m_vel = velocity;
}

void Blob::move(float dt) {
    // updates m_theta and m_phi
    m_center = m_center + (m_vel * dt);

    if (m_center.x() - m_radius < -1){
      m_vel = vec3(-1*m_vel.x(), m_vel.y(), m_vel.z());
      m_center = vec3(-1 + m_radius, m_center.y(), m_center.z());
    }
    else if (m_center.x() + m_radius > 1){
      m_vel = vec3(-1*m_vel.x(), m_vel.y(), m_vel.z());
      m_center = vec3(1 - m_radius, m_center.y(), m_center.z());
    }

    if (m_center.y() - m_radius < -1){
      m_vel = vec3(m_vel.x(), -1*m_vel.y(), m_vel.z());
      m_center = vec3(m_center.x(), -1 + m_radius, m_center.z());
    }
    else if (m_center.y() + m_radius > 1){
      m_vel = vec3(m_vel.x(), -1*m_vel.y(), m_vel.z());
      m_center = vec3(m_center.x(), 1 - m_radius, m_center.z());
    }

    if (m_center.z() - m_radius < -1){
      m_vel = vec3(m_vel.x(), m_vel.y(), -1*m_vel.z());
      m_center = vec3(m_center.x(), m_center.y(), -1 + m_radius);
    }
    else if (m_center.z() + m_radius > 1){
      m_vel = vec3(m_vel.x(), m_vel.y(), -1*m_vel.z());
      m_center = vec3(m_center.x(), m_center.y(), 1 - m_radius);
    }
}
