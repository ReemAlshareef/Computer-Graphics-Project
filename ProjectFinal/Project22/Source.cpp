
#define _CRT_SECURE_NO_DEPRECATE 
#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>
#include <MMSystem.h>
#include <GL\glew.h>  // A cross-platform open-source C/C++ extension loading library
#include <stdlib.h>
#include <GL\freeglut.h>   // handle the window-managing operations
#include <iostream>
#include <GL\glut.h>

float xArray[300];
float yArray[300];

using namespace std;

float transValueX;
float transValueY;
float transValueZ;
float toZ;
float toY;
float toX;
float nearp;
float farp;


char imagePath[] = "/Users/ralsh/Documents/CS39_6/CG_Lec/8k_earth_daymap.bmp";
char imagePath1[] = "/Users/ralsh/Documents/CS39_6/CG_Lec/8k_moon.bmp";
GLfloat light_position[] = { -8, 5, 10, 1.0 };
GLfloat shininess[] = { 8 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLint win_width = 500,
win_hight = 500;

//Makes the image into a texture, and returns the id of the texture

GLuint LoadTexture(const char* filename, int width, int height) {
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file
    file = fopen(filename, "rb");

    if (file == NULL) {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
        return 0;
    }
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    // reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }
    glGenTextures(1, &texture);            //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /////////////////////////////////////////

    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    return texture; //return whether it was successfull  
}

static void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        transValueX = transValueX - 0.1;
        cout << "The Translate X= " << transValueX << "\n";
        break;
    case GLUT_KEY_RIGHT:
        transValueX = transValueX + 0.1;
        cout << "The Translate X= " << transValueX << "\n";
        break;
    case GLUT_KEY_UP:
        transValueY = transValueY + 0.1;
        cout << "The Translate Y= " << transValueY << "\n";
        break;
    case GLUT_KEY_DOWN:
        transValueY = transValueY - 0.1;
        cout << "The Translate Y= " << transValueY << "\n";
        break;
    default:
        break;

    }
}

static void key(unsigned char keyPressed, int x, int y) // key handling
{
    switch (keyPressed) {

    case 'X':
        toX = toX + 0.1; //‘Z’ to increase the value of transValueZ by 0.1
        cout << "to X : " << toX << endl;
        break;

    case 'x':
        toX = toX - 0.1; //‘z’ to decrement the value of transValueZ by 0.1
        cout << "to x: " << toX << endl;
        break;
    case 'Y':
        toY = toY + 0.1; //‘Z’ to increase the value of transValueZ by 0.1
        cout << "to Y : " << toY << endl;
        break;

    case 'y':
        toY = toY - 0.1; //‘z’ to decrement the value of transValueZ by 0.1
        cout << "to y: " << toY << endl;
        break;
    case 'Z':
        toZ = toZ + 0.1; //‘Z’ to increase the value of transValueZ by 0.1
        cout << "to Z : " << toZ << endl;
        break;

    case 'z':
        toZ = toZ - 0.1; //‘z’ to decrement the value of transValueZ by 0.1
        cout << "to z: " << toZ << endl;
        break;

    case 'N':
        nearp = nearp + 1.0; //‘N’ to increase the value of near by 0.1
        cout << "The translate N : " << nearp << endl;
        break;

    case 'n':
        nearp = nearp - 1.0; //‘n’ to decrement the value of near by 0.1
        cout << "The translate n : " << nearp << endl;
        break;

    case 'F':
        farp = farp + 1.0; //‘F’ to increase the value of far by 0.1
        cout << "The translate F : " << farp << endl;
        break;

    case 'f':
        farp = farp - 1.0; //‘f’ to decrement the value of far by 0.1
        cout << "The translate f : " << farp << endl;
        break;
    case 's':
        shininess[0] += 2;
        cout << "Light pos s: " << shininess[0] << endl;
        break;
    case 'S':
        shininess[0] -= 2;
        cout << "Light pos S: " << shininess[0] << endl;
        break;
    case 'b':
        light_position[0] += 1;
        cout << "Light pos b: " << light_position[0] << endl;
        break;
    case 'B':
        light_position[0] -= 1;
        cout << "Light pos B: " << light_position[0] << endl;
        break;

    case 'q':
    case 27:
        exit(0);
        break;

    default:
        fprintf(stderr, "\nKeyboard commands:\n\n"
            "q, <esc> - Quit\n");
        break;

        //----------------------
    }
}
GLuint textureId; //The id of the texture
GLuint textureId1;
GLUquadric* quad;
float rotateY;

void init() {
    transValueX = 1;
    transValueY = 1;
    transValueZ = -25;
    toX = 1;
    toY = 1;
    toZ = -40;
    nearp = 1;
    farp = 200;

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);*/

    glMatrixMode(GL_PROJECTION);
    gluPerspective(65, (win_width / win_hight), 0.01, 50);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    textureId = LoadTexture(imagePath, 8192, 4096);

    if (textureId == -1) {
        cout << "Error in loading the texture" << endl;
    }
    else
        cout << "The texture value is: " << textureId << endl;

    textureId1 = LoadTexture(imagePath1, 8192, 4096);
    if (textureId1 == -1) {
        cout << "Error in loading the texture" << endl;
    }
    else
        cout << "The texture value is: " << textureId1 << endl;

    quad = gluNewQuadric();
}

void handleResize(GLint new_width, GLint new_hight) {
    /*glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float)w / (float)h, nearp, farp);*/
    win_width = new_width;
    win_hight = new_hight;
    glViewport(0, 0, win_width, win_hight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (float)win_width / (float)win_hight, nearp, farp);
}

void drawStars(void) {

    glPointSize(2.0f);

    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    for (int i = 0; i < 100; i++)
    {
        glVertex2f(xArray[i], yArray[i]);
    }
    glEnd();

}


void drawSphere() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw stars
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawStars();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glPushMatrix();
    //drawStars();
    //glPopMatrix();

    //----ENABLES-------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPushMatrix();

    glEnable(GL_DEPTH_TEST);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(50, (500 / 500), nearp, farp);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushMatrix();
    gluLookAt(toX, toY, toZ, 0, 0, 0, 0, 1, 0);

    //-----EARTH-----
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);*/

    glTranslatef(transValueX, transValueY, -16.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glRotatef(100, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 0.0f, 1.0f);
    gluQuadricTexture(quad, 1);
    gluSphere(quad, 2, 20, 20);

    //------MOON------

    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);*/

    glTranslatef(transValueX + 2, transValueY + 5, 0.5);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 0.0f, 1.0f);
    gluQuadricTexture(quad, 1);
    gluSphere(quad, 0.5, 20, 20);

    //-----DISABLES------
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glFlush();
    glutSwapBuffers();

}

void play(int value) {
    rotateY += 2.0f;
    if (rotateY > 360.f) {
        rotateY -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(25, play, 0);
}

int main(int argc, char** argv) {
    for (int i = 0; i < 100; i++)
    {

        float x = (0.9 - -0.9) * ((((float)rand()) / (float)RAND_MAX)) + (-0.9);;
        float y = (0.9 - -0.9) * ((((float)rand()) / (float)RAND_MAX)) + (-0.9);;

        xArray[i] = x;
        yArray[i] = y;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Textures-codeincodeblock.blogspot.com");
    init();
    glutTimerFunc(25, play, 0);
    glutDisplayFunc(drawSphere);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);

    sndPlaySound(TEXT("Space Sounds.wav"), SND_ASYNC | SND_LOOP); //Background sound
    glutReshapeFunc(handleResize);

    glutMainLoop();
    return 0;
}