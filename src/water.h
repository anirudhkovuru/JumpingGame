#include "main.h"

#ifndef WATER_H
#define WATER_H


class Water
{
public:
public:
    Water() {}
    Water(float x, float y, color_t color, double r);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    double rad;
private:
    VAO *object;
};

#endif // WATER_H
