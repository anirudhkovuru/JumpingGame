#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H


class Trampoline
{
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color, double r);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    double rad;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // TRAMPOLINE_H
