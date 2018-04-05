#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color, double r) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    rad = r;

    static GLfloat vertex_buffer_data[9000] = {0.0f};
    int n = 1000;
    float angle = (2*M_PI)/n;

    for (int i=0 ; i<n ; i++)
    {
        for (int j=1 ; j<3 ; j++)
        {
            vertex_buffer_data[9*i + 3*j] = this->rad*cos(-(i+j-1)*angle);
            vertex_buffer_data[9*i + 3*j + 1] = this->rad*sin(-(i+j-1)*angle);
        }
    }

    this->object = create3DObject(GL_TRIANGLES, 3*n/2, vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Trampoline::bounding_box() {
    float x = this->position.x, y = this->position.y - this->rad/2;
    bounding_box_t bbox = { x, y, 2*this->rad + 0.2, this->rad };
    return bbox;
}

