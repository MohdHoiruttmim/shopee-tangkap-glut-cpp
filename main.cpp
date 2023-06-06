#include <GL/glut.h>
#include <iostream>
#include <cstdlib> // Untuk fungsi rand()
#include <math.h>

//ukuran halaman permainan 400px x 300px

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Basket dimensions
const float BASKET_WIDTH = 100.0f;
const float BASKET_HEIGHT = 20.0f;
float BASKET_SPEED = 40.0f;

// Object dimensions
const float OBJECT_RADIUS = 20.0f;
float OBJECT_SPEED = 4.0f;

// Basket position
float basketX = 0.0f;

// Object position
float objectX;
float objectY = HEIGHT / 2;

bool objectCaught = false;
int random;

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void drawBasket(float x, float y)
{
    std::cout << x << " | ";
    std::cout << y << std::endl;
    glBegin(GL_QUADS);
        glVertex2f(x-BASKET_WIDTH/1.5, y);
        glVertex2f(x-BASKET_WIDTH/2.5, y);
        glVertex2f(x-BASKET_WIDTH/2.5, y+100.0f);
        glVertex2f(x-BASKET_WIDTH/1.5, y+100.0f);
    glEnd();
    glRectf(x - BASKET_WIDTH / 2, y - BASKET_HEIGHT / 2, x + BASKET_WIDTH / 2, y + BASKET_HEIGHT / 2);
}

void circle(float x, float y)
{
   glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159 / 180.0;
        float objectPosX = x + OBJECT_RADIUS * cos(angle);
        float objectPosY = y + OBJECT_RADIUS * sin(angle);
        glVertex2f(objectPosX, objectPosY);
    }
    glEnd();
}

void square(float x, float y)
{
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - OBJECT_RADIUS / 2, y + OBJECT_RADIUS / 2);
        glVertex2f(x - OBJECT_RADIUS / 2, y - OBJECT_RADIUS / 2);
        glVertex2f(x + OBJECT_RADIUS / 2, y - OBJECT_RADIUS / 2);
        glVertex2f(x + OBJECT_RADIUS / 2, y + OBJECT_RADIUS / 2);
    glEnd();
}

void triangle(float x, float y)
{
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y + OBJECT_RADIUS / 2);
        glVertex2f(x - OBJECT_RADIUS / 2, y - OBJECT_RADIUS / 2);
        glVertex2f(x + OBJECT_RADIUS / 2, y - OBJECT_RADIUS / 2);
    glEnd();
}

void drawObject(float x, float y, int randValue)
{
    if(randValue == 0) {
        square(x, y);
        return;
    } else if (randValue == 1) {
        circle(x, y);
        return;
    } else {
        triangle(x, y);
        return;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw basket
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBasket(basketX, -HEIGHT / 2 + BASKET_HEIGHT);

    // Draw object
    if (!objectCaught) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawObject(objectX, objectY, random);
    }

    glutSwapBuffers();
}

void update(int value)
{
    // Update object position
    if (!objectCaught) {
        objectY -= OBJECT_SPEED;

        // Check if object is caught by the basket
        if (objectY - OBJECT_RADIUS <= -HEIGHT / 2 + BASKET_HEIGHT && objectX >= basketX - BASKET_WIDTH / 2 && objectX <= basketX + BASKET_WIDTH / 2) {
            objectCaught = true;
            OBJECT_SPEED += 0.2f;
            random = rand() % 3;
            BASKET_SPEED += 10.0f;
            std::cout << "Object Caught!" << std::endl;
        }

        // Check if object is missed
        if (objectY - OBJECT_RADIUS <= -HEIGHT / 2) {
            objectCaught = false;
            std::cout << "Object Missed!" << std::endl;
            return;
        }
    }

    // Generate random object position
    if (objectCaught) {
        objectX = rand() % (int)(WIDTH - 2 * OBJECT_RADIUS) - (WIDTH / 2 - OBJECT_RADIUS);
        objectY = HEIGHT / 2;
        objectCaught = false;
    }

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void keyboard(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_LEFT:
        basketX -= BASKET_SPEED;
        break;
    case GLUT_KEY_RIGHT:
        basketX += BASKET_SPEED;
        break;
    }
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Catch the Object");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(10, update, 0);
    glutSpecialFunc(keyboard);
    glutReshapeFunc(reshape);

//    srand(ftime(NULL)); // Inisialisasi generator angka acak
    glutMainLoop();

    return 0;
}
