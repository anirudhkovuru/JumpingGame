#include "spikes.h"
#include "main.h"

Spikes::Spikes(float x, float y, double s, double x1, double x2, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    speed = s;
    l = x1;
    r = x2;

    static GLfloat vertex_buffer_data[] = {
        x-.2, y, 0.0,  //vertex1
        x+.2, y, 0.0,  //vertex2
        x, y+.6, 0.0,   //vertex3

        x-.6, y, 0.0,  //vertex1
        x-.2, y, 0.0,  //vertex2
        x-.4, y+.6, 0.0,   //vertex3

        x+.6, y, 0.0,  //vertex1
        x+.2, y, 0.0,  //vertex2
        x+.4, y+.6, 0.0,   //vertex3
    };

    this->object = create3DObject(GL_TRIANGLES, 9, vertex_buffer_data, color, GL_FILL);
}

void Spikes::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Spikes::tick() {
    this->position[0] -= speed;
    if (this->position[0] < l) {
        speed = -speed;
    }
    if (this->position[0] > r) {
        speed = -speed;
    }
}

bounding_box_t Spikes::bounding_box() {
    float x = this->position[0] + 2.1, y = this->position[1] - 1.65;
    bounding_box_t bbox = { x, y, .8, 0.6 };
    return bbox;
}

