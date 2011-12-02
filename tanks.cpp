#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>

#include <GL/glew.h>
#include <GL/glut.h>

#include "libk3d/k3d.h"
#include "libtanks/TankGameModel.h"
#include "libtanks/TankGameView.h"
#include "libtanks/KeyboardController.h"

using k3d::gl;

const int FPS = 30; // How often the screen is updated (Frames Per Second)
const int GAMERATE = 60;

TankGameModel game;                             // Game State
TankGameView view(&game);                       // Renderer
KeyboardController controller(&game, &view);    // Interface to interact with game and view

void init()
{
    if(gl::initialize("shaders/tanks.vs", "shaders/tanks.fs") == false) {
        std::cout << "k3d::gl::initialize() failed\n";
        exit(1);
    }

    srand(time(NULL));

    if (game.loadLevel("levels/simple.lvl") == false) {
        std::cout << "game.loadLevel() failed\n";
        exit(1);
    }

    if (view.loadModels("models/floor.obj",
                    "models/wall.obj",
                    "models/tank_head.obj",
                    "models/tank_treads.obj") == false) {
        std::cout << "view.loadModels() failed\n";
        exit(1);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view.renderFrame();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) { // ESC
        exit(EXIT_SUCCESS);
    }
    else {
        controller.keydown(key);
    }
}

void keyboard_up(unsigned char key, int x, int y)
{
    controller.keyup(key);
}

void display_timer(int v)
{
    glutPostRedisplay(); // XXX This is how its done everywhere else, but why can't we just call display() here ?
    glutTimerFunc(1000/v, display_timer, v);
}

void game_timer(int v)
{
    game.step();
    glutTimerFunc(1000/v, game_timer, v);
}

void reshape (int w, int h)
{
    float ratio = (float)h/(float)w;
    glViewport(0, 0, w, h);
    gl::mProjection.loadIdentity();
    gl::mProjection.perspective(-1.0, 1.0, 1.0*ratio, -1.0*ratio, -1.0, -40.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glewInit(); // I'm going to kill myself the next time I forget this

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutTimerFunc(100, display_timer, FPS);
    glutTimerFunc(100, game_timer, GAMERATE);

    glutMainLoop();
    return 0;
}
