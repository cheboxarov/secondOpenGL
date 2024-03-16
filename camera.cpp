#include "camera.h"
#include <QOpenGLShaderProgram>
Camera::Camera()
{
    m_scale = 1;
    m_position = QVector3D(0, 0, 0);
    m_globalTransform.setToIdentity();
    m_viewMatrix.setToIdentity();
    m_rotationMatrix.setToIdentity();
}

void Camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(functions != nullptr)
        return;
    m_viewMatrix.setToIdentity();
    //m_viewMatrix.translate(m_position);
    m_viewMatrix *= m_rotationMatrix;
    m_viewMatrix.rotate(m_rotation);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix *= m_globalTransform.inverted();
    program->setUniformValue("u_viewMatrix", m_viewMatrix);
}

void Camera::setScale(const float &scale)
{
    m_scale *= scale;
}

void Camera::setPosition(const QVector3D &position)
{
    m_position = position;
}

void Camera::translate(const QVector3D &position)
{
    setPosition(m_position + position);
}

void Camera::setRotation(const QQuaternion &quat)
{
    m_rotation = quat;
}

void Camera::setRotation(const QMatrix4x4 &rotationMatrix)
{
    m_rotationMatrix = rotationMatrix;
}

void Camera::rotate(const QQuaternion &quat)
{
    m_rotation = quat * m_rotation;
}

void Camera::lookAt(const QVector3D &coord)
{
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity();
    rotationMatrix.lookAt(m_position,
                          coord,
                          QVector3D(0,1,0));
    setRotation(rotationMatrix);
}

