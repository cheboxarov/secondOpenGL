#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Transfom.h>
#include <QOpenGLBuffer>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLTexture>

struct VertexData
{
    VertexData()
    {

    }
    VertexData(QVector3D p, QVector2D t, QVector3D n) :
        position(p), texCoord(t), normal(n)
    {}
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

class SceneObject : public Transform
{
public:
    explicit SceneObject(const QVector<VertexData> &vertexes,
                         const QVector<GLuint> &indexes,
                         const QImage &texture);
    virtual ~SceneObject();
    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions) override;

    void setScale(const float &scale) override;
    float getScale() const { return m_scale; };

    void setPosition(const QVector3D &position) override;
    void translate(const QVector3D &position) override;
    QVector3D getPosition() const { return m_position; };

    void setRotation(const QQuaternion &quat) override;
    void rotate(const QQuaternion &quat) override;
    QQuaternion getRotation() const { return m_rotation; }

    void setGlobalTransform(const QMatrix4x4 &globalTransform) override
    { m_globalTransform = globalTransform; }
private:
    QOpenGLBuffer m_vertexesBuffer;
    QOpenGLBuffer m_indexesBuffer;
    QOpenGLTexture*  m_texture;
    QQuaternion m_rotation;
    QVector3D m_position;
    float m_scale;
    QMatrix4x4 m_globalTransform;
};

#endif // SCENEOBJECT_H
