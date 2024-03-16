#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include <Transfom.h>

class Camera;
class SceneObject;

template <std::three_way_comparable T>
class BoundedValue {
public:
    constexpr BoundedValue(T value, T min, T max) : min_(min), max_(max) {
        SetValue(value);
    }
    constexpr inline void SetValue(T value) {
        if (value < min_)
            value = min_;
        else if (value > max_)
            value = max_;
        value_ = value;
    }
    constexpr operator T() const {  // NOLINT(google-explicit-constructor)
        return value_;
    }

    constexpr inline BoundedValue<T>& operator=(T value) {
        SetValue(value);
        return *this;
    }

    [[nodiscard]] constexpr T min() const { return min_; }
    [[nodiscard]] constexpr T max() const { return max_; }

private:
    T value_;
    T min_;
    T max_;
};


class CameraController : public Transform
{
public:
    CameraController(Camera* camera);
    void SetLongitude(const float phi);
    void AddLongitude(const float phi)
    { SetLongitude((phi * (m_sens / 500.0)) + phi_); }
    void SetLatitude(const float theta);
    void AddLatitude(const float theta)
    { SetLatitude((theta * (m_sens / 500.0)) + theta_); }
    void setSens(const float &sens);
    void translate(const QVector3D &position) override
    { setPosition(m_position + position); }
    void setPosition(const QVector3D &position) override;
    void setGlobalTransform(const QMatrix4x4 &globalTransform) override;
    void setRadius(const float &radius);
    float radius() const { return radius_; }

    //MOVING
    void moveFront();
    void moveBack();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void setMoveSpeed(const float &speed) { m_moveSpeed = speed; }
    float speed() const { return m_moveSpeed; }

    void setCameraObject(SceneObject* object);

private:
    using BFloat = BoundedValue<float>;
    static constexpr float pi  = 3.14159;
    static constexpr float pi2 = pi * 2;

    static constexpr auto initial_radius = BFloat(1.0f, 0.01f, 1000.0f);
    static constexpr auto initial_phi    = BFloat(0.0f, -pi * 0.49, pi * 0.49);
    static constexpr auto initial_theta  = BFloat(0.0f, -pi, pi);

    BFloat radius_ = initial_radius;
    BFloat phi_    = initial_phi;
    BFloat theta_  = initial_theta;
    void replaceCamera();
    Camera* m_camera;
    float m_sens;
    QVector3D m_position;
    QMatrix4x4 m_globalTransform;
    float m_moveSpeed;
    SceneObject* m_cameraObject;
};

#endif // CAMERACONTROLLER_H
