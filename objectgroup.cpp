#include "objectgroup.h"

ObjectGroup::ObjectGroup()
{
    m_scale = 1;
    m_globalTransform.setToIdentity();
}

void ObjectGroup::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{

    for(auto obj : m_objects)
    {
        obj->draw(program, functions);
    }
}

void ObjectGroup::setScale(const float &scale)
{
    m_scale = scale;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_position);
    localMatrix.rotate(m_rotation);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;
    for(auto obj : m_objects)
    {
        obj->setGlobalTransform(localMatrix);
    }
}

void ObjectGroup::setPosition(const QVector3D &position)
{
    m_position = position;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_position);
    localMatrix.rotate(m_rotation);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;
    for(auto obj : m_objects)
    {
        obj->setGlobalTransform(localMatrix);
    }
}

void ObjectGroup::setRotation(const QQuaternion &quat)
{
    m_rotation = quat;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_position);
    localMatrix.rotate(m_rotation);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;
    for(auto obj : m_objects)
    {
        obj->setGlobalTransform(localMatrix);
    }
}

void ObjectGroup::addObject(Transform *object)
{
    m_objects.push_back(object);
}
