#include "oglwidget.h"
#include <Transfom.h>
#include <camera.h>
#include <sceneobject.h>
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QFile>
#include <thread>
#include <mutex>
#include <QLabel>
#include <QOpenGLTexture>
#include <skybox.h>
#include <cameracontroller.h>
#include <QKeyEvent>
#include <objectgroup.h>
#include <QWheelEvent>

std::mutex mtx;

OGLWidget::OGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    m_camera = new Camera();
    m_cameraContoroller = new CameraController(m_camera);
    m_cameraContoroller->setSens(3);
    m_cameraContoroller->setRadius(0);
    m_cameraContoroller->translate(QVector3D(0,1,0));
}

OGLWidget::~OGLWidget()
{
    delete m_camera;
    for(SceneObject* object : m_objects)
    {
        delete object;
    }
}

void OGLWidget::initializeGL()
{
    glClearColor(0,1,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    initShaders();
    SceneObject* floor = initCube(100, QImage(":/textures/grass.jpg"));
    floor->translate(QVector3D(0,-50,0));
    m_transforms.push_back(floor);
    m_skyBox = new SkyBox(QImage(":/skyboxes/skybox1.png"), 300);
    auto obj = loadObj(":/models/rabbit/Rabbit_Lowpoly_1.obj");
    auto obj2 = loadObj(":/models/rabbit/Rabbit_Lowpoly_3.obj");
    obj2->translate(QVector3D(2,0,0));
    ObjectGroup* group1 = createGroup();
    group1->addObject(obj);
    group1->addObject(obj2);
    group1->translate(QVector3D(2,0,0));
    m_transforms.append(group1);
    SceneObject* cubeMok = initCube(1,QImage(":/textures/malou.jpg"));
    m_cameraContoroller->setCameraObject(cubeMok);
    m_transforms.push_back(cubeMok);
    m_timer.start(16, this);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programSkyBox.bind();
    m_programSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_camera->draw(&m_programSkyBox);
    m_skyBox->draw(&m_programSkyBox, context()->functions());
    m_programSkyBox.release();

    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_lightPower", 0.5f);
    m_program.setUniformValue("lightPosition", QVector4D(0,0,0,1));
    m_camera->draw(&m_program);
    for(Transform* tObject : m_transforms)
    {
        tObject->draw(&m_program, context()->functions());
    }

    m_program.release();
}

void OGLWidget::resizeGL(int w, int h)
{
    float aspect = w / (float)h;
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(100, aspect, 0.1, 500);
}

void OGLWidget::initShaders()
{
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/main.vsh"))
    {
        qDebug() << "Error to load main.vsh";
        exit(1);
    }
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/main.fsh"))
    {
        qDebug() << "Error to load main.fsh";
        exit(1);
    }
    if(!m_program.link())
    {
        qDebug() << "Error ling main.vsh with main.fsh";
        exit(1);
    }
    if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh"))
    {
        qDebug() << "ERROR COMPILE SHADER: VSHADER";
        exit(1);
    }
    if(!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh"))
    {
        qDebug() << "ERROR COMPILE SHADER: FSHADER";
        exit(1);
    }
    if(!m_programSkyBox.link())
    {
        qDebug() << "ERROR OF LINK";
        exit(1);
    }
}

SceneObject *OGLWidget::initCube(float size, QImage &&texture)
{
    float width_div_2 = size / 2.0f;
    QVector<VertexData> vertexes;
    std::thread th1([&vertexes, &width_div_2](){
        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0,1), QVector3D(0, 0, 1)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0,0), QVector3D(0, 0, 1)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1,1), QVector3D(0, 0, 1)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1,0), QVector3D(0, 0, 1)));

        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0,1), QVector3D(1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0,0), QVector3D(1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1,1), QVector3D(1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1,0), QVector3D(1, 0, 0)));

        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0,1), QVector3D(0, 1, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0,0), QVector3D(0, 1, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1,1), QVector3D(0, 1, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1,0), QVector3D(0, 1, 0)));

        vertexes.push_back(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0,1), QVector3D(0, 0, -1)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(0,0), QVector3D(0, 0, -1)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1,1), QVector3D(0, 0, -1)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1,0), QVector3D(0, 0, -1)));

        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0,1), QVector3D(-1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0,0), QVector3D(-1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1,1), QVector3D(-1, 0, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1,0), QVector3D(-1, 0, 0)));

        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0,1), QVector3D(0, -1, 0)));
        vertexes.push_back(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0,0), QVector3D(0, -1, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1,1), QVector3D(0, -1, 0)));
        vertexes.push_back(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1,0), QVector3D(0, -1, 0)));
    });
    QVector<GLuint> indexes;
    std::thread th2([&indexes](){
        for(int i = 0; i < 24; i+=4)
        {
            indexes.push_back(i + 0);
            indexes.push_back(i + 1);
            indexes.push_back(i + 2);
            indexes.push_back(i + 2);
            indexes.push_back(i + 1);
            indexes.push_back(i + 3);
        }
    });
    th1.join();
    th2.join();
    mtx.lock();
    SceneObject* cube = new SceneObject(vertexes, indexes, texture.mirrored());
    mtx.unlock();
    m_objects.push_back(cube);
    return cube;
}

SceneObject *OGLWidget::loadObj(const QString path)
{
    QFile objFile(path);
    if (!objFile.exists())
    {
        qDebug() << "Object file not found:" << path;
        return nullptr;
    }
    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    while(!input.atEnd())
    {
        QString str = input.readLine();
        QStringList strList = str.split(" ");
        if (strList[0] == "#")
        {
            continue;
        }
        else if(strList[0] == "mtllib")
        {
            qDebug() << "File materials" << str;
            continue;
        }
        else if(strList[0] == "v")
        {
            coords.append(QVector3D(strList[1].toFloat(), strList[2].toFloat(), strList[3].toFloat()));
            continue;
        }
        else if (strList[0] == "vt")
        {
            texCoords.append(QVector2D(strList[1].toFloat(), strList[2].toFloat()));
            continue;
        }
        else if(strList[0] == "vn")
        {
            normals.append(QVector3D(strList[1].toFloat(), strList[2].toFloat(), strList[3].toFloat()));
            continue;
        }
        else if (strList[0] == "f")
        {
            for(int i = 1; i <= 3; ++i)
            {
                QStringList vert = strList[i].split("/");
                if (vert.size() != 3 || (vert[0].isEmpty() || vert[1].isEmpty() || vert[2].isEmpty()))
                {
                    if(vert[1].isEmpty())
                    {
                        vertexes.append(VertexData(coords[vert[0].toLong() - 1], QVector2D(0,0), normals[vert[2].toLong() - 1]));
                        indexes.append(indexes.size());
                    }
                    else
                    {
                        qDebug() << "Error indexes read";
                        return nullptr;
                    }
                }
                else
                {
                    vertexes.append(VertexData(coords[vert[0].toLong() - 1], texCoords[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]));
                    indexes.append(indexes.size());
                }
            }
            continue;
        }
    }

    objFile.close();
    mtx.lock();

    auto obj = new SceneObject(vertexes, indexes, m_textures.grass);
    mtx.unlock();
    m_objects.append(obj);
    return obj;
}

ObjectGroup *OGLWidget::createGroup()
{
    ObjectGroup *group = new ObjectGroup;
    m_groups.push_back(group);
    return group;
}

void OGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint diff = m_mousePos - ev->pos();
    m_mousePos = ev->pos();
    m_cameraContoroller->AddLatitude(-diff.x());
    m_cameraContoroller->AddLongitude(-diff.y());
    update();
}

void OGLWidget::mousePressEvent(QMouseEvent *ev)
{
    m_mousePos = ev->pos();
}

void OGLWidget::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
    case Qt::Key_W:
        m_moving = MoveState::FRONT;
        break;
    case Qt::Key_S:
        m_moving = MoveState::BACK;
        break;
    case Qt::Key_A:
        m_moving = MoveState::LEFT;
        break;
    case Qt::Key_D:
        m_moving = MoveState::RIGHT;
        break;
    case Qt::Key_Space:
        m_moving = MoveState::UP;
        break;
    case Qt::Key_Shift:
        m_moving = MoveState::DOWN;
        break;
    default:
        m_moving = MoveState::STAY;
        break;
    }
}

void OGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
    m_moving = MoveState::STAY;
}

void OGLWidget::timerEvent(QTimerEvent *ev)
{
    switch (m_moving) {
    case MoveState::FRONT:
        m_cameraContoroller->moveFront();
        break;
    case MoveState::BACK:
        m_cameraContoroller->moveBack();
        break;
    case MoveState::LEFT:
        m_cameraContoroller->moveLeft();
        break;
    case MoveState::RIGHT:
        m_cameraContoroller->moveRight();
        break;
    case MoveState::UP:
        m_cameraContoroller->moveUp();
        break;
    case MoveState::DOWN:
        m_cameraContoroller->moveDown();
        break;
    default:
        break;
    }
    update();
}

void OGLWidget::wheelEvent(QWheelEvent *ev)
{
    if(ev->angleDelta().y() < 0)
    {
        m_cameraContoroller->setRadius(m_cameraContoroller->radius() + 0.5);
    }
    else
    {
        m_cameraContoroller->setRadius(m_cameraContoroller->radius() - 0.5);
    }
}

