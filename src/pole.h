#include "main.h"

#ifndef POLE_H
#define POLE_H


class Pole
{
public:
    Pole() {}
    Pole(float x, float y, color_t color, double r);
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // POLE_H
