#ifndef TEXTURES_H
#define TEXTURES_H
#include <QImage>
class Textures {
public:
    Textures()
    {
        grass = QImage(":/textures/grass.jpg");
        tanya = QImage(":/textures/tanya.jpg");
    }
    QImage grass;
    QImage tanya;
};

#endif // TEXTURES_H
