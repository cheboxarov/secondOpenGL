#ifndef CAMERA_H
#define CAMERA_H

#include <Transfom.h>

class Camera : public Transform
{
public:
    Camera();
    virtual ~Camera() {};

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions = nullptr) override;

    void setScale(const float &scale) override;
    float getScale() const { return m_scale; };

    void setPosition(const QVector3D &position) override;
    void translate(const QVector3D &position) override;
    QVector3D getPosition() const { return m_position; };

    void setRotation(const QQuaternion &quat) override;
    void setRotation(const QMatrix4x4 &rotationMatrix) override;
    void rotate(const QQuaternion &quat) override;
    QQuaternion getRotation() const { return m_rotation; }

    void lookAt(const QVector3D &coord);

    void setGlobalTransform(const QMatrix4x4 &globalTransform) override
    { m_globalTransform = globalTransform; }

private:
    void replaceViewMatrix();
    QQuaternion m_rotation;
    QVector3D m_position;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_rotationMatrix;
};

#endif // CAMERA_H
