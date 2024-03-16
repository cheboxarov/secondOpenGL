#include "skybox.h"
#include <sceneobject.h>

SkyBox::SkyBox(const QImage &texture, float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0, 2.0/3.0), QVector3D(0, 0, -1)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0, 1.0/3.0), QVector3D(0, 0, -1)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0/4.0, 2.0/3.0), QVector3D(0, 0, -1)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0/4.0, 1.0/3.0), QVector3D(0, 0, -1)));

    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0/4.0, 2.0/3.0), QVector3D(-1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0/4.0, 1.0/3.0), QVector3D(-1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(-1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(-1, 0, 0)));

    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(2.0/4.0, 1), QVector3D(0, -1, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(0, -1, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0/4.0, 1), QVector3D(0, -1, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(0, -1, 0)));

    vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0/4.0, 2.0/3.0), QVector3D(0, 0, 1)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(0, 0, 1)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(0, 0, 1)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(0, 0, 1)));

    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0, 2.0/3.0), QVector3D(1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0/4.0, 2.0/3.0), QVector3D(1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0, 1.0/3.0), QVector3D(1, 0, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(1, 0, 0)));

    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0/4.0, 0), QVector3D(0, 1, 0)));
    vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0/4.0, 1.0/3.0), QVector3D(0, 1, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(2.0/4.0, 0), QVector3D(0, 1, 0)));
    vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0/4.0, 1.0/3.0), QVector3D(0, 1, 0)));

    QVector<GLuint> indexes;
    for(int i = 0; i < 24; i+=4)
    {
        indexes.push_back(i + 0);
        indexes.push_back(i + 2);
        indexes.push_back(i + 1);
        indexes.push_back(i + 2);
        indexes.push_back(i + 3);
        indexes.push_back(i + 1);
    }

    m_cube = new SceneObject(vertexes, indexes, texture);
}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program, functions);
}
