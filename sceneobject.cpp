#include "sceneobject.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <thread>

SceneObject::SceneObject(const QVector<VertexData> &vertexes,
                         const QVector<GLuint> &indexes,
                         const QImage &texture) :
    m_indexesBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr),
    m_scale(1)
{
    if(m_vertexesBuffer.isCreated())
        m_vertexesBuffer.destroy();
    if(m_indexesBuffer.isCreated())
        m_indexesBuffer.destroy();
    if(m_texture != nullptr)
    {
        if(m_texture->isCreated())
        {
            delete m_texture;
            m_texture = nullptr;
        }
    }
    m_vertexesBuffer.create();
    m_vertexesBuffer.bind();
    m_vertexesBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    m_vertexesBuffer.release();
    m_indexesBuffer.create();
    m_indexesBuffer.bind();
    m_indexesBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexesBuffer.release();
    m_texture = new QOpenGLTexture(texture.mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
    m_globalTransform.setToIdentity();
}

SceneObject::~SceneObject()
{
    if(m_vertexesBuffer.isCreated())
        m_vertexesBuffer.destroy();
    if(m_indexesBuffer.isCreated())
        m_indexesBuffer.destroy();
    if(m_texture != nullptr)
    {
        if(m_texture->isCreated())
            m_texture->destroy();
    }
}

void SceneObject::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexesBuffer.isCreated() || !m_indexesBuffer.isCreated())
        return;
    m_texture->bind(0);
    program->setUniformValue("u_texture", 0);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_position);
    modelMatrix.rotate(m_rotation);
    modelMatrix.scale(m_scale);
    modelMatrix = modelMatrix * m_globalTransform;
    program->setUniformValue("u_modelMatrix", modelMatrix);

    m_vertexesBuffer.bind();
    int offset = 0;
    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLoc);
    program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normLoc);
    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexesBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexesBuffer.size(), GL_UNSIGNED_INT, 0);

    m_vertexesBuffer.release();
    m_indexesBuffer.release();
    m_texture->release();

}

void SceneObject::setScale(const float &scale)
{
    m_scale *= scale;
}

void SceneObject::setPosition(const QVector3D &position)
{
    m_position = position;
}

void SceneObject::translate(const QVector3D &position)
{
    setPosition(m_position += position);
}

void SceneObject::setRotation(const QQuaternion &quat)
{
    m_rotation = quat;
}

void SceneObject::rotate(const QQuaternion &quat)
{
    m_rotation = quat * m_rotation;
}
