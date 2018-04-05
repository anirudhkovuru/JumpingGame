#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "trampoline.h"
#include "pole.h"
#include "magnet.h"
#include "spikes.h"
#include "water.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball player, balls[30];
Trampoline t;
Pole p;
Magnet m;
Spikes s;
Water w;
Floor f;
int score = 0;
int gameover = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    f.draw(VP);
    w.draw(VP);
    player.draw(VP);
    for (int i=0 ; i<30 ; i++)
    {
        balls[i].draw(VP);
    }
    t.draw(VP);
    p.draw(VP);
    m.draw(VP);
    s.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int pan_up = glfwGetKey(window, GLFW_KEY_UP);
    int pan_down = glfwGetKey(window, GLFW_KEY_DOWN);
    int pan_left = glfwGetKey(window, GLFW_KEY_LEFT);
    int pan_right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int zoom_in = glfwGetKey(window, GLFW_KEY_Z);
    int zoom_out = glfwGetKey(window, GLFW_KEY_X);
    int q = glfwGetKey(window, GLFW_KEY_Q);

    if (left) {
        if (player.position[0] > -5.6)
        {
            player.speedx = 0.1;
            player.accelaratex = 0.1;
        }
        else if (player.position[0] >= w.position[0] - w.rad && player.position[0] <= w.position[0] + w.rad) {
            player.speedx = 0.00005;
            player.accelaratex = 0.1;
        }
    }
    if (right) {
        if (player.position[0] < 5.6)
        {
            player.speedx = -0.1;
            player.accelaratex = -0.1;
        }
        else if (player.position[0] >= w.position[0] - w.rad && player.position[0] <= w.position[0] + w.rad) {
            player.speedx = -0.05;
            player.accelaratex = -0.1;
        }
    }
    if (up) {
        if (player.position[1] == -3) {
            player.speedy = 0.2;
        }
        else if (player.position[1] < -3) {
            player.speedy = 0.1;
        }
    }
    if (pan_up) {
        screen_center_y += 0.1;
    }
    if (pan_down) {
        screen_center_y -= 0.1;
    }
    if (pan_left) {
        screen_center_x -= 0.1;
    }
    if (pan_right) {
        screen_center_x += 0.1;
    }
    if (zoom_in) {
        screen_zoom *= 1.01;
    }
    if (zoom_out) {
        screen_zoom /= 1.01;
    }
    if (q) {
        quit(window);
    }
}

void tick_elements() {
    stringstream convert;
    convert << score;
    string str = "Score = " + convert.str();
    glfwSetWindowTitle(window, str.c_str());
    reset_screen();
    if (player.position[1] > -3.3) {
        player.accelaratey = 0.01;
    }
    else {
        if (player.speedy >= 0) {
            player.accelaratey = 0.01;
        }
        else if (player.speedy < 0) {
            player.accelaratey = -0.01;
        }
    }
    //player tick...
    player.tick();
    //player within bounds...
    if (player.position[1] < -3 && (player.position[0] <= w.position[0] - w.rad + .3 || player.position[0] >= w.position[0] + w.rad - .3)) {
        player.set_position(player.position[0], -3);
    }
    if (player.position[0] < -5.6) {
        player.set_position(-5.6, player.position[1]);
        player.speedx = 0;
    }
    if (player.position[0] > 5.6) {
        player.set_position(5.6, player.position[1]);
        player.speedx = 0;
    }

    //water physics...
    if (player.position[1] <= -3 && player.position[0] >= w.position[0] - w.rad && player.position[0] <= w.position[0] + w.rad) {
        float a = pow(player.position[0] - w.position[0], 2);
        float b = pow(w.rad - player.rad, 2);
        float c = -3.3 - sqrt(b-a);
        if (player.position[1] <= c) {
            player.set_position(player.position[0], c);
            player.speedy = 0;
            if (player.position[1] >= -3.3 - w.rad + player.rad && player.speedy == 0) {
                if (player.position[0] > w.position[0]) {
                    player.speedx = 0.01;
                }
                if (player.position[0] < w.position[0]) {
                    player.speedx = -0.01;
                }
            }
        }
    }

    //balls tick and collision detection with player...
    for (int i=0 ; i<30 ; i++)
    {
        balls[i].tick();
        if (balls[i].position[0] >= 7)
        {
            if (i<10)
            {
                balls[i] = Ball(-7, rand() % 2 -1, -0.009 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_RED);
            }
            else if (i>=10 && i<20)
            {
                balls[i] = Ball(-7, rand() % 2 +1, -0.008 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_DARK_GREEN);
            }
            else if (i>=20)
            {
                balls[i] = Ball(-7, rand() % 2 +3, -0.007 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_YELLOW);
            }
        }
        if (detect_collision(player.bounding_box(), balls[i].bounding_box())) {
            if (balls[i].position[1] < player.position[1] && player.speedy < 0)
            {
                player.speedy = .2;
                if (i<10)
                {
                    score += 100;
                    balls[i] = Ball(-7, rand() % 2 -1, -0.009 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_RED);
                }
                else if (i>=10 && i<20)
                {
                    score += 200;
                    balls[i] = Ball(-7, rand() % 2 +1, -0.008 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_DARK_GREEN);
                }
                else if (i>=20)
                {
                    score += 300;
                    balls[i] = Ball(-7, rand() % 2 +3, -0.007 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_YELLOW);
                }
            }
        }
    }

    //spikes tick...
    s.tick();
    if (detect_collision(player.bounding_box(), s.bounding_box())) {
        player.set_position(-2, -3);
        player.speedx = 0;
        player.accelaratex = 0;
        gameover = 1;
    }

    //trampoline collision with player...
    if (detect_collision(player.bounding_box(), t.bounding_box())) {
        if (player.position[1] > t.position[1]) {
            player.speedy = .4;
        }
        else {
            if (player.position[0] > t.position[0]) {
                player.set_position(player.position[0]+.1, player.position[1]);
                player.speedx = 0;
            }
            else if (player.position[0] < t.position[0]) {
                player.set_position(player.position[0]-.1, player.position[1]);
                player.speedx = 0;
            }
        }
    }

    //player in magnet region...
    m.count++;
    if (m.count == 120) {
        int dec = rand() % 2;
        if (dec == 0) {
            m = Magnet(-3, rand() % 4, 0.02, COLOR_DARK_RED);
        }
        else {
            m = Magnet(9, rand() % 4, 0.02, COLOR_DARK_RED);
        }
    }
    if (player.position[1] >= m.position[1]-1.5 && player.position[1] <= m.position[1]) {
        if (m.position[0] == -3) {
            player.accelaratex = -m.power;
        }
        else if (m.position[0] == 9) {
            player.accelaratex = m.power;
        }
    }
    else {
        player.accelaratex = 0;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player = Ball(-2, -3, 0, 0, 0.3, 0.01, COLOR_BLACK);

    for (int i=0 ; i<30 ; i++)
    {
        if (i<10)
        {
            balls[i] = Ball(-7, rand() % 2 -1, -0.009 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_RED);
        }
        else if (i>=10 && i<20)
        {
            balls[i] = Ball(-7, rand() % 2 +1, -0.008 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_DARK_GREEN);
        }
        else if (i>=20)
        {
            balls[i] = Ball(-7, rand() % 2 +3, -0.007 - double(rand() % 4)/100.0, 0, double(rand() % 3)/10.0 +0.3, 0, COLOR_YELLOW);
        }
    }

    f = Floor(COLOR_GREEN);
    t = Trampoline(-4, -2.7, COLOR_BROWN, 0.6);
    p = Pole(-4, -2.7, COLOR_BROWN, 0.6);
    m = Magnet(-3, rand() % 4, 0.02, COLOR_DARK_RED);
    s = Spikes(2, -1.65, 0.03, 1, 3, COLOR_PURPLE);
    w = Water(0, -3.3, COLOR_DARK_BLUE, 1.5);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BLUE.r / 256.0, COLOR_BLUE.g / 256.0, COLOR_BLUE.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick() && !gameover) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        /*if (gameover && count == 0) {
            cout << "Score = " << score << "\n";
            count++;
        }*/

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);

}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 <= (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 <= (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 6 / screen_zoom;
    float bottom = screen_center_y - 6 / screen_zoom;
    float left   = screen_center_x - 6 / screen_zoom;
    float right  = screen_center_x + 6 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
