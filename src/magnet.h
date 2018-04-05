#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet
{
public:
    Magnet() {}
    Magnet(float x, float y, double att, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    double power;
    int count;
private:
    VAO *object;
};

#endif // MAGNET_H
