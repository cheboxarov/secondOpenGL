#ifndef SKYBOX_H
#define SKYBOX_H

#include <Transfom.h>

class QImage;
class SceneObject;

class SkyBox : public Transform
{
public:
    SkyBox(const QImage &texture, float width);
    virtual ~SkyBox();
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    void setRotation(const QQuaternion &r) override { (void)r; }
    void setRotation(const QMatrix4x4 &rotationMatrix) override { (void)rotationMatrix; }
    void rotate(const QQuaternion &r) override { (void)r; }
    void setPosition(const QVector3D &vec) override { (void)vec; }
    void translate(const QVector3D &vec) override { (void)vec; }
    void setScale(const float &scaleFactor) override { (void)scaleFactor; }
private:
    SceneObject *m_cube;
};

#endif // SKYBOX_H
