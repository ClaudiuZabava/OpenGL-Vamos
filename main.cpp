#include <windows.h>  
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> 
#include <GL/freeglut.h> 
#include "loadShaders.h"
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
float PI = 3.141592;

GLuint
VaoId,
VaoId2,
VaoId3,
VaoId4,
VaoId5,
VaoId6,
VaoId7,
VaoId8,
VaoId9,
VaoId10,
VaoId11,
VaoId12,
VaoId13,
VaoId14,

VboId1,
EboId1,
VboId2,
EboId2,
VboId3,
VboId4,
VboId5,
VboId6,
EboId3,
EboId5,
EboId6,
EboId4,
VboId7,
VboId8,
VboId9,
VboId10,
EboId7,
EboId8,
EboId9,
EboId10,
VboId11,
EboId11,
VboId12,
EboId12,
VboId13,
EboId13,
VboId14,
EboId14,



ProgramId,
myMatrixLocation,
matrUmbraLocation,
viewLocation,
projLocation,
matrRotlLocation,
oldLocation,
codColLocation,
lightColorLoc,
lightPosLoc,
viewPosLoc,
codCol;

// elemente pentru matricea de vizualizare si matricea de proiectie
float Obsx, Obsy, Obsz;
float Refx = 300.0f, Refy = 400.0f, Refz = 100.0f;
float Vx = 0.0, Vy = 0.0, Vz = 1.0;
float alpha = 0.22f, beta = 1.65f, dist = 1060.0f;
float incr_alpha1 = 0.01f, incr_alpha2 = 0.01f;
float width = 800, height = 600, znear = 0.1, fov = 45;
bool isFalling = false, isStopped = false;


float const U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;
int const NR_PARR = 10, NR_MERID = 20;
float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;
float radius = 50;

int index, index_aux;
float i1 = 0.0f, alpha1 = 0.0f, i2 = 0.0f, alpha2 = 0.0f, i3 = 0.0f, alpha3 = 0.0f, i4 = 0.0f;

// sursa de lumina
float xL = 500.f, yL = 0.f, zL = 400.f;

// matrice
glm::mat4 view, projection, matrUmbra, matrRot1, matrRotDefault, matrTranslDefault, oldView, matrTransl2, matrTranslModif;

void movement(void)
{

	i1 = i1 + alpha1;
	if (i1 < -1.57)
	{
		alpha1 = 0.0f;
	}
	if (i1 <= -0.75)
	{
		if(isFalling == false && i2 <= 690)
			alpha2 = 5.0f;
	}

	if (!isFalling)
	{
		i2 = i2 + alpha2;
		i3 = i2 / 4;
		i4 = i2 / 3;
	}
	if (i2 >=690)
	{
		isFalling = true;
	}
	if (isFalling == true && isStopped == false)
	{
		alpha2 = 0.0f;
		alpha3 = -3.0f;
		i2 = i2 + alpha3;
		i3 = i3 + alpha3;
		i4 = i3;
		if (i4 <= 5)
		{
			isStopped = true;
			alpha2 = 0.0f;
			alpha3 = 0.0f;
		}
	}


	glutPostRedisplay();

}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		dist -= 5.0;
		break;
	case '+':
		dist += 5.0;
		break;
	}
	if (key == 27)
		exit(0);
	//cout << "distanta de la observator la origine: " << dist << endl;
}
void keyPress(int button, int state, int x, int y)
{
	switch (button) {

	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			alpha1 = -0.05f;
		}
		glutIdleFunc(movement);
		break;
	default:
		break;
	}
}
void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		beta -= 0.01f;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.01f;
		break;
	case GLUT_KEY_UP:
		alpha += incr_alpha1;
		if (abs(alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.01f;
		}
		break;
	case GLUT_KEY_DOWN:
		alpha -= incr_alpha2;
		if (abs(alpha + PI / 2) < 0.05)
		{
			incr_alpha2 = 0.f;
		}
		else
		{
			incr_alpha2 = 0.01f;
		}
		break;
	}
}
void CreateVBO(void) // sunt folosite doua buffere
{
	// varfurile pentru cub si "ground" 
	GLfloat Vertices1[] =
	{
		// coordonate                   // culori			    // normale
		-50.0f,  -50.0f, 50.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  -1.0f, -1.0f, -1.0f,
		50.0f,  -50.0f,  50.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  1.0f, -1.0f, -1.0f,
		50.0f,  50.0f,  50.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, -1.0f,
		-50.0f,  50.0f, 50.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, -1.0f,
		-50.0f,  -50.0f, 150.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f, -1.0f, -1.0f, 1.0f,
		50.0f,  -50.0f,  150.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f, 1.0f, -1.0f, 1.0f,
		50.0f,  50.0f,  150.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, 1.0f,
		-50.0f,  50.0f, 150.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, 1.0f,
		// 
	   -1000.0f,  -1000.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1000.0f,  -1000.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1000.0f,  1000.0f,  0.0f, 1.0f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	   -1000.0f,  1000.0f,  0.0f, 1.0f,  0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};
	// indicii pentru varfuri
	GLubyte Indices1[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	  9, 10, 8, 10, 8, 11
	};


	// se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(1, &VboId1);
	glGenBuffers(1, &EboId1);

	glBindBuffer(GL_ARRAY_BUFFER, VboId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);


	// se activeaza atributele
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glEnableVertexAttribArray(2); // atributul 2 = normale
}


void CreateVAO2(void)
{
	// CUBUL 
	// 
	GLfloat Vertices2[] =
	{
		// coordonate                   // culori			    // normale
		-20.0f, -30.0f, 115.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  -1.0f, -1.0f, -1.0f,
		20.0f, -30.0f, 115.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  1.0f, -1.0f, -1.0f,
		20.0f,  30.0f, 115.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, -1.0f,
		-20.0f,  30.0f, 115.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, -1.0f,
		-20.0f, -30.0f, 215.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f, -1.0f, -1.0f, 1.0f,
		20.0f, -30.0f, 215.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f, 1.0f, -1.0f, 1.0f,
		20.0f,  30.0f, 215.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, 1.0f,
		-20.0f,  30.0f, 215.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, 1.0f,
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId2);
	glBindVertexArray(VaoId2);
	glGenBuffers(1, &VboId2); // atribute
	glGenBuffers(1, &EboId2); // indici

	glBindBuffer(GL_ARRAY_BUFFER, VboId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 2 = normale
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
}
// picior
void CreateVAO3(void)
{
	// CUBUL 
	// 
	GLfloat Vertices2[] =
	{
		-10.0f, -30.0f, 5.0f, 1.0f,
		 10.0f, -30.0f, 5.0f, 1.0f,
		 10.0f,  -10.0f, 5.0f, 1.0f,
		-10.0f,  -10.0f, 5.0f, 1.0f,
		-10.0f, -30.0f, 115.0f, 1.0f,
		 10.0f, -30.0f, 115.0f, 1.0f,
		 10.0f,  -10.0f, 115.0f, 1.0f,
		-10.0f,  -10.0f, 115.0f, 1.0f
	};

	GLfloat Colors2[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId3);
	glBindVertexArray(VaoId3);
	glGenBuffers(1, &VboId3); // atribute
	glGenBuffers(1, &EboId3); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2) + sizeof(Colors2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices2), Vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices2), sizeof(Colors2), Colors2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices2)));

}

// picior
void CreateVAO4(void)
{
	GLfloat Vertices3[] =
	{
		-10.0f, 10.0f, 5.0f, 1.0f,
		 10.0f, 10.0f, 5.0f, 1.0f,
		 10.0f,  30.0f, 5.0f, 1.0f,
		-10.0f,  30.0f, 5.0f, 1.0f,
		-10.0f, 10.0f, 115.0f, 1.0f,
		 10.0f, 10.0f, 115.0f, 1.0f,
		 10.0f,  30.0f, 115.0f, 1.0f,
		-10.0f,  30.0f, 115.0f, 1.0f
	};

	GLfloat Colors3[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices3[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId4);
	glBindVertexArray(VaoId4);
	glGenBuffers(1, &VboId4); // atribute
	glGenBuffers(1, &EboId4); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices3) + sizeof(Colors3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices3), Vertices3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices3), sizeof(Colors3), Colors3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices3), Indices3, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices3)));

}

//mana stanga
void CreateVAO5(void)
{
	GLfloat Vertices3[] =
	{
		-10.0f, 40.0f, 135.0f, 1.0f,
		 10.0f, 40.0f, 135.0f, 1.0f,
		 10.0f,  60.0f, 135.0f, 1.0f,
		-10.0f,  60.0f, 135.0f, 1.0f,
		-10.0f, 30.0f, 215.0f, 1.0f,
		 10.0f, 30.0f, 215.0f, 1.0f,
		 10.0f,  50.0f, 215.0f, 1.0f,
		-10.0f,  50.0f, 215.0f, 1.0f
	};

	GLfloat Colors3[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices3[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId5);
	glBindVertexArray(VaoId5);
	glGenBuffers(1, &VboId5); // atribute
	glGenBuffers(1, &EboId5); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices3) + sizeof(Colors3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices3), Vertices3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices3), sizeof(Colors3), Colors3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices3), Indices3, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices3)));

}

//mana dreapta
void CreateVAO6(void)
{

	GLfloat Vertices3[] =
	{
		-10.0f, -60.0f, 135.0f, 1.0f,
		 10.0f, -60.0f, 135.0f, 1.0f,
		 10.0f,  -40.0f, 135.0f, 1.0f,
		-10.0f,  -40.0f, 135.0f, 1.0f,
		-10.0f, -50.0f, 215.0f, 1.0f,
		 10.0f, -50.0f, 215.0f, 1.0f,
		 10.0f,  -30.0f, 215.0f, 1.0f,
		-10.0f,  -30.0f, 215.0f, 1.0f
	};

	GLfloat Colors3[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices3[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId6);
	glBindVertexArray(VaoId6);
	glGenBuffers(1, &VboId6); // atribute
	glGenBuffers(1, &EboId6); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices3) + sizeof(Colors3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices3), Vertices3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices3), sizeof(Colors3), Colors3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices3), Indices3, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices3)));

}


void AssociateAttributePointers()
{
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId1);
	glDeleteBuffers(1, &EboId1);
	glDeleteBuffers(1, &VboId2);
	glDeleteBuffers(1, &EboId2);
	glDeleteBuffers(1, &VboId3);
	glDeleteBuffers(1, &EboId3);
	glDeleteBuffers(1, &VboId4);
	glDeleteBuffers(1, &EboId4);
	glDeleteBuffers(1, &VboId5);
	glDeleteBuffers(1, &EboId5);
	glDeleteBuffers(1, &VboId6);
	glDeleteBuffers(1, &EboId6);
	glDeleteBuffers(1, &VboId7);
	glDeleteBuffers(1, &EboId7);
	glDeleteBuffers(1, &VboId8);
	glDeleteBuffers(1, &EboId8);
	glDeleteBuffers(1, &VboId9);
	glDeleteBuffers(1, &EboId9);
	glDeleteBuffers(1, &VboId10);
	glDeleteBuffers(1, &EboId10);
	glDeleteBuffers(1, &VboId11);
	glDeleteBuffers(1, &EboId11);
	glDeleteBuffers(1, &VboId12);
	glDeleteBuffers(1, &EboId12);
	glDeleteBuffers(1, &VboId13);
	glDeleteBuffers(1, &EboId13);
	glDeleteBuffers(1, &VboId14);
	glDeleteBuffers(1, &EboId14);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
	glDeleteVertexArrays(1, &VaoId2);
	glDeleteVertexArrays(1, &VaoId3);
	glDeleteVertexArrays(1, &VaoId4);
	glDeleteVertexArrays(1, &VaoId5);
	glDeleteVertexArrays(1, &VaoId6);
	glDeleteVertexArrays(1, &VaoId7);
	glDeleteVertexArrays(1, &VaoId8);
	glDeleteVertexArrays(1, &VaoId9);
	glDeleteVertexArrays(1, &VaoId10);
	glDeleteVertexArrays(1, &VaoId11);
	glDeleteVertexArrays(1, &VaoId12);
	glDeleteVertexArrays(1, &VaoId13);
	glDeleteVertexArrays(1, &VaoId14);
}

// Poarta
void CreateVAO7(void)
{
	// bara 
	// 
	GLfloat Vertices2[] =
	{
		690.0f, -330.0f, 0.0f, 1.0f,
		 710.0f, -330.0f, 0.0f, 1.0f,
		 710.0f,  -310.0f, 0.0f, 1.0f,
		690.0f,  -310.0f, 0.0f, 1.0f,
		690.0f, -330.0f, 355.0f, 1.0f,
		 710.0f, -330.0f, 355.0f, 1.0f,
		 710.0f,  -310.0f, 355.0f, 1.0f,
		690.0f,  -310.0f, 355.0f, 1.0f
	};

	GLfloat Colors2[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId7);
	glBindVertexArray(VaoId7);
	glGenBuffers(1, &VboId7); // atribute
	glGenBuffers(1, &EboId7); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2) + sizeof(Colors2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices2), Vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices2), sizeof(Colors2), Colors2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices2)));

}

void CreateVAO8(void)
{
	// bara 
	// 
	GLfloat Vertices2[] =
	{
		690.0f, 330.0f, 0.0f, 1.0f,
		 710.0f, 330.0f, 0.0f, 1.0f,
		 710.0f,  310.0f, 0.0f, 1.0f,
		690.0f,  310.0f, 0.0f, 1.0f,
		690.0f, 330.0f, 355.0f, 1.0f,
		 710.0f, 330.0f, 355.0f, 1.0f,
		 710.0f,  310.0f, 355.0f, 1.0f,
		690.0f,  310.0f, 355.0f, 1.0f
	};

	GLfloat Colors2[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId8);
	glBindVertexArray(VaoId8);
	glGenBuffers(1, &VboId8); // atribute
	glGenBuffers(1, &EboId8); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2) + sizeof(Colors2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices2), Vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices2), sizeof(Colors2), Colors2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices2)));

}


void CreateVAO9(void)
{
	// transversala
	// 
	GLfloat Vertices2[] =
	{
		690.0f, -330.0f, 355.0f, 1.0f,
		 710.0f, -330.0f, 355.0f, 1.0f,
		 710.0f,  330.0f, 355.0f, 1.0f,
		690.0f,  330.0f, 355.0f, 1.0f,
		690.0f, -330.0f, 385.0f, 1.0f,
		 710.0f, -330.0f, 385.0f, 1.0f,
		 710.0f,  330.0f, 385.0f, 1.0f,
		690.0f,  330.0f, 385.0f, 1.0f
	};

	GLfloat Colors2[] =
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId9);
	glBindVertexArray(VaoId9);
	glGenBuffers(1, &VboId9); // atribute
	glGenBuffers(1, &EboId9); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2) + sizeof(Colors2), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices2), Vertices2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices2), sizeof(Colors2), Colors2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices2)));

}

// Cap

void CreateVAO10(void)
{
	// SFERA
	// Matricele pentru varfuri, culori, indici
	glm::vec4 Vertices1[(NR_PARR + 1) * NR_MERID];
	glm::vec3 Colors1[(NR_PARR + 1) * NR_MERID];
	GLushort Indices1[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = U_MIN + parr * step_u; // valori pentru u si v
			float v = V_MIN + merid * step_v;
			float x_vf = radius * cosf(u) * cosf(v); // coordonatele varfului corespunzator lui (u,v)
			float y_vf = radius * cosf(u) * sinf(v);
			float z_vf = radius * sinf(u);

			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			index = merid * (NR_PARR + 1) + parr;
			Vertices1[index] = glm::vec4(x_vf, y_vf, z_vf + 480, 2.0);
			Colors1[index] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(v), 0.1f + -1.5 * sinf(u));
			Indices1[index] = index;

			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (NR_MERID)+merid;
			Indices1[(NR_PARR + 1) * NR_MERID + index_aux] = index;

			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index; // varful v considerat
				int index2 = index + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}
				Indices1[AUX + 4 * index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices1[AUX + 4 * index + 1] = index2;
				Indices1[AUX + 4 * index + 2] = index3;
				Indices1[AUX + 4 * index + 3] = index4;
			}
		}
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId10);
	glBindVertexArray(VaoId10);
	glGenBuffers(1, &VboId10); // atribute
	glGenBuffers(1, &EboId10); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1) + sizeof(Colors1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices1), Vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices1), sizeof(Colors1), Colors1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices1)));


}


// minge 
void CreateVAO11(void)
{
	// SFERA
	// Matricele pentru varfuri, culori, indici
	glm::vec4 Vertices1[(NR_PARR + 1) * NR_MERID];
	glm::vec3 Colors1[(NR_PARR + 1) * NR_MERID];
	GLushort Indices1[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementarea reprezentarii parametrice 
			float u = U_MIN + parr * step_u; // valori pentru u si v
			float v = V_MIN + merid * step_v;
			float x_vf = radius * cosf(u) * cosf(v); // coordonatele varfului corespunzator lui (u,v)
			float y_vf = radius * cosf(u) * sinf(v);
			float z_vf = radius * sinf(u);

			// identificator ptr varf; coordonate + culoare + indice la parcurgerea meridianelor
			index = merid * (NR_PARR + 1) + parr;
			Vertices1[index] = glm::vec4(x_vf + 225, y_vf, z_vf + 55, 2.3);
			Colors1[index] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(v), 0.1f + -1.5 * sinf(u));
			Indices1[index] = index;

			// indice ptr acelasi varf la parcurgerea paralelelor
			index_aux = parr * (NR_MERID)+merid;
			Indices1[(NR_PARR + 1) * NR_MERID + index_aux] = index;

			// indicii pentru desenarea fetelor, pentru varful curent sunt definite 4 varfuri
			if ((parr + 1) % (NR_PARR + 1) != 0) // varful considerat sa nu fie Polul Nord
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index; // varful v considerat
				int index2 = index + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = index + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}
				Indices1[AUX + 4 * index] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices1[AUX + 4 * index + 1] = index2;
				Indices1[AUX + 4 * index + 2] = index3;
				Indices1[AUX + 4 * index + 3] = index4;
			}
		}
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId11);
	glBindVertexArray(VaoId11);
	glGenBuffers(1, &VboId11); // atribute
	glGenBuffers(1, &EboId11); // indici

	// legare+"incarcare" buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1) + sizeof(Colors1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices1), Vertices1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices1), sizeof(Colors1), Colors1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(Vertices1)));


}

void CreateVAO12(void)
{

	// CUBUL 
	// 
	GLfloat Vertices2[] =
	{
		// coordonate                   // culori			    // normale
		710.0f, -300.0f, 385.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  -1.0f, -1.0f, -1.0f,
		900.0f, -300.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  1.0f, -1.0f, -1.0f,
		900.0f,  300.0f, 0.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, -1.0f,
		710.0f,  300.0f, 385.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, -1.0f,
		710.0f, -300.0f, 385.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f, -1.0f, -1.0f, 1.0f,
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3,
	  2, 3, 4,   4, 3, 0,
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId12);
	glBindVertexArray(VaoId12);
	glGenBuffers(1, &VboId12); // atribute
	glGenBuffers(1, &EboId12); // indici

	glBindBuffer(GL_ARRAY_BUFFER, VboId12);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId12);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 2 = normale
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
}


void CreateVAO13(void)
{

	// CUBUL 
	// 
	GLfloat Vertices2[] =
	{
		// coordonate                   // culori			    // normale
		710.0f, -300.0f, 385.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  -1.0f, -1.0f, -1.0f,
		710.0f, -300.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  1.0f, -1.0f, -1.0f,
		900.0f, -300.0f, 0.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, -1.0f,
		710.0f,  -300.0f, 385.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, -1.0f,
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId13);
	glBindVertexArray(VaoId13);
	glGenBuffers(1, &VboId13); // atribute
	glGenBuffers(1, &EboId13); // indici

	glBindBuffer(GL_ARRAY_BUFFER, VboId13);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId13);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 2 = normale
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
}

void CreateVAO14(void)
{

	// CUBUL 
	// 
	GLfloat Vertices2[] =
	{
		// coordonate                   // culori			    // normale
		710.0f, 300.0f, 385.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  -1.0f, -1.0f, -1.0f,
		710.0f, 300.0f, 0.0f, 1.0f,  0.0f, 0.5f, 0.9f, 0.5f,  1.0f, -1.0f, -1.0f,
		900.0f, 300.0f, 0.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, 1.0f, 1.0f, -1.0f,
		710.0f,  300.0f, 385.0f, 1.0f,   0.0f, 0.5f, 0.9f, 0.5f, -1.0f, 1.0f, -1.0f,
	};

	GLushort Indices2[] =
	{
	  1, 2, 0,   2, 0, 3
	};

	// generare VAO/buffere
	glGenVertexArrays(1, &VaoId14);
	glBindVertexArray(VaoId14);
	glGenBuffers(1, &VboId14); // atribute
	glGenBuffers(1, &EboId14); // indici

	glBindBuffer(GL_ARRAY_BUFFER, VboId14);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId14);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	// atributele; 
	glEnableVertexAttribArray(0); // atributul 0 = pozitie
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1); // atributul 1 = culoare
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // atributul 2 = normale
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
}





void CreateShaders(void)
{
	ProgramId = LoadShaders("Gazon.vert", "Gazon.frag");
	glUseProgram(ProgramId);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}
void Initialize(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
	CreateShaders();
	CreateVBO();
	CreateVAO2();
	CreateVAO3();
	CreateVAO4();
	CreateVAO5();
	CreateVAO6();
	CreateVAO7();
	CreateVAO8();
	CreateVAO9();
	CreateVAO10();
	CreateVAO11();
	CreateVAO12();
	CreateVAO13();
	CreateVAO14();
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	lightColorLoc = glGetUniformLocation(ProgramId, "lightColor");
	lightPosLoc = glGetUniformLocation(ProgramId, "lightPos");
	viewPosLoc = glGetUniformLocation(ProgramId, "viewPos");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// reperul de vizualizare + proiectie
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);
	//cout << "Obsx=" << Obsx << " Obsy=" << Obsy << " Obsz=" << Obsz << endl;
	//Obsx = 166.274f;
	//Obsy = 1354.45f;
	//Obsx = 366.731;
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz);
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz);
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// Variabile uniforme pentru iluminare
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, xL, yL, zL);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	// matricea pentru umbra
	float D = -5.f;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

	// desenare obiecte opace
	glBindVertexArray(VaoId);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	// buffer 1
	glBindBuffer(GL_ARRAY_BUFFER, VboId1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId1);
	AssociateAttributePointers();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)(36)); // "ground" 

	// buffer 2
	glBindVertexArray(VaoId2);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 4
	glBindVertexArray(VaoId4);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 5
	glBindVertexArray(VaoId5);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 6
	glBindVertexArray(VaoId6);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 7
	glBindVertexArray(VaoId7);
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 8
	glBindVertexArray(VaoId8);
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 9
	glBindVertexArray(VaoId9);
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	//buffer 10
	glBindVertexArray(VaoId10);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}
	
	
	// desenare obiecte TRANSPARENTE 
	
	glEnable(GL_BLEND);
	//glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA); // de testat alte variante https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml si factori-destinatie: GL_ONE, GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA
	
	// plasa-mare
	glBindVertexArray(VaoId12);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0);  // plasa

	// plasa-stanga
	glBindVertexArray(VaoId13);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);  // plasa

	// dreapta
	glBindVertexArray(VaoId14);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);  // plasa
	
	
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);



	// umbra 2
	glBindVertexArray(VaoId2);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// umbra 4
	glBindVertexArray(VaoId4);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// umbra 5
	glBindVertexArray(VaoId5);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// umbra 6
	glBindVertexArray(VaoId6);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// umbra cap
	glBindVertexArray(VaoId10);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	//umbra buffer 3
	glBindVertexArray(VaoId3);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));
















	//=====================================================================================!!!!=============================================================
	//==========================================  Aici se lucreaza la miscarea obiectelor 3d, afectand matricile de proiectie / view =======================
	// ================================== Daca vreti sa creati alte VAO si sa le adaugati, dati paste deasupra acestie linii, NU dedesubt ==================


	oldView = view;
	oldLocation = viewLocation;

	//matrRot1 = glm::rotate(glm::mat4(1.0f), i1, glm::vec3(-0.115, -0.033, 0.5));
	matrRotDefault = glm::rotate(glm::mat4(1.0f), 3.14f, glm::vec3(0.0, -50.0, 0.5));
	matrTranslDefault = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 120.0));
	matrRot1 = glm::rotate(glm::mat4(1.0f), i1, glm::vec3(0.0, 50.0, 0.5));

	view = view * matrTranslDefault * matrRotDefault * matrRot1;
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//buffer 3
	glBindVertexArray(VaoId3);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	view = oldView;
	viewLocation = oldLocation;
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// inline function to choose i3 if isFalling is true otherwise to choose i3


	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(i2, i3, i4));

	view = view * matrTransl2;
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//buffer 11 - minge
	glBindVertexArray(VaoId11);
	codCol = 3;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}


	view = oldView;
	viewLocation = oldLocation;
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	////////////////////////////////////////////////////PORTAR///////////////////////////////////////////////////////////////////



	matrTranslModif = glm::translate(glm::mat4(1.0f), glm::vec3(550.0, 0.0, 0.0));

	view = view * matrTranslModif;
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	// buffer 2
	glBindVertexArray(VaoId2);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	//buffer 3
	glBindVertexArray(VaoId3);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 4
	glBindVertexArray(VaoId4);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 5
	glBindVertexArray(VaoId5);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

	// buffer 6
	glBindVertexArray(VaoId6);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));


	//buffer 10
	glBindVertexArray(VaoId10);
	codCol = 2;
	glUniform1i(codColLocation, codCol);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	view = oldView;
	viewLocation = oldLocation;
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);






	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////SPECTATOR////////////////////////////////////////////////////////////

	for (float contor = -400.0f; contor <= 400.0f; contor += 150.0f)
	{
		matrTranslModif = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0, contor, 0.0));

		view = view * matrTranslModif;
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


		// buffer 2
		glBindVertexArray(VaoId2);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

		//buffer 3
		glBindVertexArray(VaoId3);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

		// buffer 4
		glBindVertexArray(VaoId4);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

		// buffer 5
		glBindVertexArray(VaoId5);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));

		// buffer 6
		glBindVertexArray(VaoId6);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (GLvoid*)(0));


		//buffer 10
		glBindVertexArray(VaoId10);
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
		{
			if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
				glDrawElements(
					GL_QUADS,
					4,
					GL_UNSIGNED_SHORT,
					(GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
		}

		view = oldView;
		viewLocation = oldLocation;
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	}





	//===============================================================================  Fara paste aici =======================================================






		//projection = oldProjection;


	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Plan 3D");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutMouseFunc(keyPress);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}