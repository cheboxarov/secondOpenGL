#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include <Transfom.h>

class ObjectGroup : public Transform
{
public:
    ObjectGroup();
    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions) override;

    void setScale(const float &scale) override;
    float getScale() const { return m_scale; };

    void setPosition(const QVector3D &position) override;
    void translate(const QVector3D &position) override
    { setPosition(m_position + position); }
    QVector3D getPosition() const { return m_position; }

    void setRotation(const QQuaternion &quat) override;
    void rotate(const QQuaternion &quat) override
    { setRotation(quat * m_rotation); }
    QQuaternion getRotation() const { return m_rotation; }

    void addObject(Transform* object);
    QVector<Transform*> getObjects() const { return m_objects; }

private:
    QQuaternion m_rotation;
    QVector3D m_position;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QVector<Transform*> m_objects;
    QMatrix4x4 localMatrix;
};

#endif // OBJECTGROUP_H
