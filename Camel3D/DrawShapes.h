#ifndef DRAWSHAPES_H
#define DRAWSHAPES_H

#ifndef VECTOR3
#include "GL/freeglut.h"
#include "GL/glut.h"
#include "Vector3.h"
#endif

void DrawCube(Vector3 min, Vector3 max){

	glBegin(GL_QUADS);

	min -= Vector3(0.05f, 0.05f, 0.05f);
	max += Vector3(0.05f, 0.05f, 0.05f);

	Vector3 min2 = min + Vector3(0.1f, 0, 0);
	Vector3 min3 = min + Vector3(0, 0.1f, 0);
	Vector3 min4 = min + Vector3(0.1f, 0.1f, 0);

	Vector3 max2 = max - Vector3(0.1f, 0, 0);
	Vector3 max3 = max - Vector3(0, 0.1f, 0);
	Vector3 max4 = max - Vector3(0.1f, 0.1f, 0);

	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min2.x, min2.y, min2.z);
	glVertex3f(min4.x, min4.y, min4.z);
	glVertex3f(min3.x, min3.y, min3.z);

	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max2.x, max2.y, max2.z);
	glVertex3f(max4.x, max4.y, max4.z);
	glVertex3f(max3.x, max3.y, max3.z);

	glVertex3f(min3.x, min3.y, min3.z);
	glVertex3f(min4.x, min4.y, min4.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max2.x, max2.y, max2.z);

	glVertex3f(min2.x, min2.y, min2.z);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max4.x, max4.y, max4.z);
	glVertex3f(max3.x, max3.y, max3.z);

	glVertex3f(min2.x, min2.y, min2.z);
	glVertex3f(min4.x, min4.y, min4.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max3.x, max3.y, max3.z);

	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min3.x, min3.y, min3.z);
	glVertex3f(max2.x, max2.y, max2.z);
	glVertex3f(max4.x, max4.y, max4.z);

	glEnd();
}

#endif