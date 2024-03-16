#ifndef TRANSFOM_H
#define TRANSFOM_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
enum class MoveState {
    STAY,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
class QOpenGLShaderProgram;
class QOpenGLFunctions;

class Transform
{
public:
    virtual ~Transform() {}
    virtual void setRotation(const QQuaternion &quat) { (void)quat; };
    virtual void setRotation(const QMatrix4x4 &rotationMatrix) { (void)rotationMatrix; };
    virtual void rotate(const QQuaternion &quat) { (void)quat; };
    virtual void setPosition(const QVector3D &position) { (void)position; };
    virtual void translate(const QVector3D &position) { (void)position; };
    virtual void setScale(const float &scaleFactor) { (void)scaleFactor; };
    virtual void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions)
    { (void)program; (void)functions; };
    virtual void setGlobalTransform(const QMatrix4x4 &globalTransform) { (void)globalTransform; };
};

#endif // TRANSFOM_H
