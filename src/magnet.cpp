#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, double att, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    power = att;
    count = 0;

    static GLfloat vertex_buffer_data_left[] = {
        x, y, 0.0,  //vertex1
        x+.5, y, 0.0,  //vertex2
        x, y-1.5, 0.0,   //vertex3
        x, y-1.5, 0.0,   //vertex3
        x+.5, y-1.5, 0.0,   //vertex4
        x+.5, y, 0.0,   //vertex2

        x+.5, y, 0.0,  //vertex1
        x+.5, y-.5, 0.0,  //vertex2
        x+2, y, 0.0,   //vertex3
        x+2, y, 0.0,   //vertex3
        x+2, y-.5, 0.0,   //vertex4
        x+.5, y-.5, 0.0,   //vertex2

        x+.5, y-1.5, 0.0,  //vertex1
        x+.5, y-1, 0.0,  //vertex2
        x+2, y-1.5, 0.0,   //vertex3
        x+2, y-1.5, 0.0,   //vertex3
        x+2, y-1, 0.0,   //vertex4
        x+.5, y-1, 0.0   //vertex2
    };

    static GLfloat vertex_buffer_data_right[] = {
        x, y, 0.0,  //vertex1
        x-.5, y, 0.0,  //vertex2
        x, y-1.5, 0.0,   //vertex3
        x, y-1.5, 0.0,   //vertex3
        x-.5, y-1.5, 0.0,   //vertex4
        x-.5, y, 0.0,   //vertex2

        x-.5, y, 0.0,  //vertex1
        x-.5, y-.5, 0.0,  //vertex2
        x-2, y, 0.0,   //vertex3
        x-2, y, 0.0,   //vertex3
        x-2, y-.5, 0.0,   //vertex4
        x-.5, y-.5, 0.0,   //vertex2

        x-.5, y-1.5, 0.0,  //vertex1
        x-.5, y-1, 0.0,  //vertex2
        x-2, y-1.5, 0.0,   //vertex3
        x-2, y-1.5, 0.0,   //vertex3
        x-2, y-1, 0.0,   //vertex4
        x-.5, y-1, 0.0   //vertex2
    };

    if (this->position[0] < 0) {
        this->object = create3DObject(GL_TRIANGLES, 18, vertex_buffer_data_left, color, GL_FILL);
    }
    else if (this->position[0] > 0) {
        this->object = create3DObject(GL_TRIANGLES, 18, vertex_buffer_data_right, color, GL_FILL);
    }
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

