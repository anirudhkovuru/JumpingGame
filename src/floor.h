#include "main.h"

#ifndef FLOOR_H
#define FLOOR_H


class Floor
{
public:
    Floor() {}
    Floor(color_t color);
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // FLOOR_H
