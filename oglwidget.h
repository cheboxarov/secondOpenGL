#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <Textures.h>
#include <QTimer>
#include <Transfom.h>

class SceneObject;
class Camera;
class Transform;
class QOpenGLTexture;
class SkyBox;
class CameraController;
class ObjectGroup;

class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget* parent = nullptr);
    virtual ~OGLWidget();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void initShaders();
    SceneObject *initCube(float size, QImage &&texture);
    SceneObject *loadObj(const QString path);

    ObjectGroup *createGroup();

    // EVENTS
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
    void keyPressEvent(QKeyEvent* ev) override;
    void keyReleaseEvent(QKeyEvent* ev) override;
    void timerEvent(QTimerEvent* ev) override;
    void wheelEvent(QWheelEvent* ev) override;

private:

    QPoint m_mousePos;
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkyBox;
    Textures m_textures;
    Camera* m_camera;
    MoveState m_moving;
    CameraController* m_cameraContoroller;
    QVector<Transform*> m_transforms;
    QVector<SceneObject*> m_objects;
    QVector<ObjectGroup*> m_groups;
    QOpenGLTexture* m_grassTexture;
    SkyBox* m_skyBox;
    QBasicTimer m_timer;
};

#endif // OGLWIDGET_H
