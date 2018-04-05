#include "pole.h"
#include "main.h"

Pole::Pole(float x, float y, color_t color, double r) {

    static GLfloat vertex_buffer_data[] = {
        x+r, y, 0.0,  //vertex1
        x+.1+r, y, 0.0,  //vertex2
        x+r, y-r, 0.0,   //vertex3
        x+r, y-r, 0.0,   //vertex3
        x+.1+r, y-r, 0.0,   //vertex4
        x+.1+r, y, 0.0,   //vertex2

        x-r, y, 0.0, //vertex1
        x-r-.1, y, 0.0, //vertex2
        x-r, y-r, 0.0, //vertex3
        x-r, y-r, 0.0, //vertex3
        x-r-.1, y-r, 0.0, //vertex4
        x-r-.1, y, 0.0 //vertex2
    };

    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, color, GL_FILL);
}

void Pole::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

