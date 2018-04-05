#include "floor.h"
#include "main.h"

Floor::Floor(color_t color)
{
    static GLfloat vertex_buffer_data[] = {
        -7.0, -3.3, 0.0,  //vertex1
        -7.0, -6.0, 0.0,  //vertex2
        7.0, -3.3, 0.0,   //vertex3
        7.0, -3.3, 0.0,   //vertex3
        7.0, -6.0, 0.0,   //vertex4
        -7.0, -6.0, 0.0   //vertex2
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Floor::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

