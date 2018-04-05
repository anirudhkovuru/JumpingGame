#include "main.h"

#ifndef SPIKES_H
#define SPIKES_H


class Spikes
{
public:
    Spikes() {}
    Spikes(float x, float y, double s, double x1, double x2, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    double speed;
    double l;
    double r;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // SPIKES_H
