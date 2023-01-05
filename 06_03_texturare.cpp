//// Sa stergi toate comentariile.
//// La linia 175 in loc de "wall.jps" descarci tu o img de pe net, o pui in directorul sursa al proiectului si ii pui numele in locul lui "wall.jps"
/// In shader.frag sa schimbi numele din codCuloare in VboChanger




#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"

using namespace std;

//////////////////////////////////////

GLuint
VaoGradient,
VboGradient,
EboGradient,
VaoTextura,
VboTextura,
EboTextura,
ColorBufferId,
ProgramId,
myMatrixLocation;

GLuint texture;

int VboChanger;

float width = 180.f, height = 160.f;
glm::mat4
myMatrix, resizeMatrix = glm::ortho(-width, width, -height, height),
Translatie = glm::translate(glm::mat4(1.0f), glm::vec3(30.0, 30.0, 0.0)),
Scalare = glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 0.5, 0.0));

void CreateVBO(void)
{
	static const GLfloat vf_pos[] =
	{
	-15.0f, -15.0f, 0.0f, 1.0f,
	 15.0f,  -15.0f, 0.0f, 1.0f,
	 15.0f,  15.0f, 0.0f, 1.0f,
	-15.0f,  15.0f, 0.0f, 1.0f,

	};
	static const GLfloat vf_col[] =
	{
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	};
	static const GLuint vf_ind[] =
	{
	0, 1, 2, 3, 0, 2
	};

	glGenBuffers(1, &VboGradient);
	glGenBuffers(1, &EboGradient);
	glGenVertexArrays(1, &VaoGradient);

	glBindVertexArray(VaoGradient);

	glBindBuffer(GL_ARRAY_BUFFER, VboGradient);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vf_col) + sizeof(vf_pos), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vf_pos), vf_pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vf_pos), sizeof(vf_col), vf_col);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboGradient);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vf_ind), vf_ind, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vf_pos));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLfloat Vertices[] = {
	  -150.0f,  -150.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // stanga jos
	   150.0f,  -150.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // dreapta jos
	   150.0f,  150.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // dreapta sus
	   -150.0f,  150.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f, 0.0f, 1.0f  // stanga sus
	};

	GLuint Indices[] = {
	  0, 1, 2, // Primul triunghi
	  0, 2, 3 //  Al doilea triunghi
	};

	glGenVertexArrays(1, &VaoTextura);
	glGenBuffers(1, &VboTextura);
	glGenBuffers(1, &EboTextura);

	glBindVertexArray(VaoTextura);

	glBindBuffer(GL_ARRAY_BUFFER, VboTextura);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboTextura);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EboGradient);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboGradient);
	glDeleteBuffers(1, &EboTextura);
	glDeleteBuffers(1, &VboTextura);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoGradient);
	glDeleteVertexArrays(1, &VaoTextura);
}

void LoadTexture(void)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image("grass-texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("06_02_Shader.vert", "06_02_Shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	CreateVBO();
	LoadTexture();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	CreateShaders();
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VaoGradient);
	myMatrix = resizeMatrix * Translatie * Scalare;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));



	glBindVertexArray(VaoTextura);
	VboChanger = 1;
	glUniform1i(glGetUniformLocation(ProgramId, "VboChanger"), VboChanger);
	myMatrix = resizeMatrix * Scalare * Translatie;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();

	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("baciu daniel mihai 343");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

