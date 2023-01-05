#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

static GLfloat angle[] = { 0.0, 0.0, 0.0 };
static GLint axis = 1;
static GLint delay = 20;

static GLuint textureName;

void update() {

    angle[axis] += 2.0;

    if (angle[axis] > 360.0)
        angle[axis] -= 360.0;

    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();

    glRotatef(angle[0], 1.0, 0.0, 0.0);
    glRotatef(angle[1], 0.0, 1.0, 0.0);
    glRotatef(angle[2], 0.0, 0.0, 1.0);
}

void mouseClicked(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON)
            axis = 0;
        else if (button == GLUT_MIDDLE_BUTTON)
            axis = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            axis = 2;
    }
}


void display() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select the texture object
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Create the sphere
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1.0, 20, 20);
    gluDeleteQuadric(quadric);
    glPopMatrix();
    glFlush();

    // Handle the double buffering
    glutSwapBuffers();
}

void reshape(int width, int height) {
    GLfloat aspect;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);

    // Clear the projection matrix
    glLoadIdentity();

    // Set the projection matrix (based on the aspect ratio)
    if (width <= height) {
        aspect = (GLfloat)height / (GLfloat)width;
        glOrtho(-2.0, 2.0, -2.0 * aspect, 2.0 * aspect, -10.0, 10.0);
    }
    else {
        aspect = (GLfloat)width / (GLfloat)height;
        glOrtho(-2.0 * aspect, 2.0 * aspect, -2.0, 2.0, -10.0, 10.0);
    }

}

void timer(int value) {
    update();
    glutPostRedisplay();
    glutTimerFunc(delay, timer, 0);
}

void readRAWImage(const char* filename, GLbyte data[512][256][3]) {
    FILE* file;

    file = fopen(filename, "rb");
    if (file != NULL) {
        fread(data, 512 * 256 * 3, 1, file);
        fclose(file);
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    // Read the "image"
    GLbyte image[512][256][3];
    readRAWImage("sc.bmp", image);

    // Get a name for the texture
    glGenTextures(1, &textureName);

    // Bind the texture object to its name
    glBindTexture(GL_TEXTURE_2D, textureName);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Create the texture object
    glTexImage2D(GL_TEXTURE_2D, 0,         
        3,                 // 3 components (i.e., RGB)
        512,               // Width
        64,               // Height
        0,                 // Border width
        GL_RGB,            // Format
        GL_UNSIGNED_BYTE,  // Data type of the texels
        image);

    glEnable(GL_TEXTURE_2D);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // GLUT Initialization
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sphere");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(delay, timer, 0);
    glutMouseFunc(mouseClicked);

    init();

    glutMainLoop();
}