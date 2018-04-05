#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, double sx, double sy, double r, double acc, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedx = sx;
    speedy = sy;
    rad = r;
    accelaratey = acc;
    accelaratex = 0;

    static GLfloat vertex_buffer_data[9000] = {0.0f};
    int n = 1000;
    float angle = (2*M_PI)/n;

    for (int i=0 ; i<n ; i++)
    {
        for (int j=1 ; j<3 ; j++)
        {
            vertex_buffer_data[9*i + 3*j] = this->rad*cos((i+j-1)*angle);
            vertex_buffer_data[9*i + 3*j + 1] = this->rad*sin((i+j-1)*angle);
        }
    }

    this->object = create3DObject(GL_TRIANGLES, 3*n, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= speedx;
    this->position.y += speedy;
    speedy -= accelaratey;
    speedx -= accelaratex;
    if (speedx < 0 && speedx + accelaratex > 0) {
        accelaratex = 0;
        speedx =0;
    }
    if (speedx > 0 && speedx + accelaratex < 0) {
        accelaratex = 0;
        speedx =0;
    }
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2*this->rad, 2*this->rad + .1 };
    return bbox;
}
