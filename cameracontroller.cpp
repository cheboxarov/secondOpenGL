#include "cameracontroller.h"
#include <camera.h>
#include <sceneobject.h>

CameraController::CameraController(Camera* camera) :
    m_camera(camera),
    m_cameraObject(nullptr)
{
    m_sens = 5;
    radius_ = 5;
    m_moveSpeed = 5;
    m_position  = QVector3D(0,0,0);
}

void CameraController::SetLongitude(const float phi)
{
    if (phi < -pi)
        phi_ = -pi;
    else if(pi < phi)
        phi_ = pi;
    else
        phi_ = phi;
    replaceCamera();
}

void CameraController::SetLatitude(const float theta)
{
    if(theta > pi)
    {
        theta_ = -pi;
    }
    else if(theta < -pi)
    {
        theta_ = pi;
    }
    else
    {
        theta_ = theta;
    }
    replaceCamera();
}

void CameraController::setSens(const float &sens)
{
    m_sens = sens;
}

void CameraController::setPosition(const QVector3D &position)
{
    m_position = position;
    replaceCamera();
}

void CameraController::setGlobalTransform(const QMatrix4x4 &globalTransform)
{
    m_globalTransform = globalTransform;
    replaceCamera();
}

void CameraController::setRadius(const float &radius)
{
    radius_ = radius;
    replaceCamera();
}

void CameraController::moveFront()
{
    float xPos = qCos(theta_) * (m_moveSpeed / 10);
    float zPos = qSin(theta_) * (m_moveSpeed / 10);
    m_position += QVector3D(-xPos, 0, -zPos);
    replaceCamera();
}

void CameraController::moveBack()
{
    float xPos = qCos(theta_) * (m_moveSpeed / 10);
    float zPos = qSin(theta_) * (m_moveSpeed / 10);
    m_position += QVector3D(xPos, 0, zPos);
    replaceCamera();
}

void CameraController::moveRight()
{
    float xPos = qCos(theta_ + (pi / 2)) * (m_moveSpeed / 10);
    float zPos = qSin(theta_ + (pi / 2)) * (m_moveSpeed / 10);
    m_position += QVector3D(-xPos, 0, -zPos);
    replaceCamera();
}

void CameraController::moveLeft()
{
    float xPos = qCos(theta_ - (pi / 2)) * (m_moveSpeed / 10);
    float zPos = qSin(theta_ - (pi / 2)) * (m_moveSpeed / 10);
    m_position += QVector3D(-xPos, 0, -zPos);
    replaceCamera();
}

void CameraController::moveUp()
{
    m_position += QVector3D(0,(m_moveSpeed / 10),0);
    replaceCamera();
}

void CameraController::moveDown()
{
    m_position += QVector3D(0,-(m_moveSpeed / 10),0);
    replaceCamera();
}

void CameraController::replaceCamera()
{
    const float x_radius = qCos(theta_) * qCos(phi_) * radius_;
    const float y_radius = qSin(phi_) * radius_;
    const float z_radius = qSin(theta_) * qCos(phi_) * radius_;
    qDebug() << phi_ << theta_ << x_radius << y_radius << z_radius;
    // m_camera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), m_theta * (m_sens / 10)));
    // m_camera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1, 0), m_phi * (m_sens / 10)));
    m_camera->setGlobalTransform( m_globalTransform );
    m_camera->setPosition(QVector3D(x_radius,y_radius,z_radius) + m_position);
    m_camera->lookAt(m_position);
    // m_camera->setRotation(rotationMatrix);
    if (m_cameraObject != nullptr)
    {
        QMatrix4x4 localMatrix;
        localMatrix.translate(m_position);
        m_cameraObject->setGlobalTransform(localMatrix);
    }
}

void CameraController::setCameraObject(SceneObject* object)
{
    m_cameraObject = object;
}
