#include "BoxCollider.h"
#include "Vector3.h"
#include "GameObject.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glut.h"

BoxCollider::BoxCollider(Vector3* pos, Vector3* size){
	max = new Vector3(pos->x + (size->x) / 2, pos->y + (size->y) / 2, pos->z + (size->z) / 2);
	min = new Vector3(pos->x - (size->x) / 2, pos->y - (size->y) / 2, pos->z - (size->z) / 2);
}

BoxCollider::~BoxCollider(){
	delete max;
	delete min;
}

void BoxCollider::DrawCollider(){
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	glBegin(GL_QUADS);

	glColor3f(0, 1, 0);

	glVertex3f(max->x, max->y, min->z);
	glVertex3f(min->x, max->y, min->z);
	glVertex3f(min->x, max->y, max->z);
	glVertex3f(max->x, max->y, max->z);

	glVertex3f(max->x, min->y, max->z);
	glVertex3f(min->x, min->y, max->z);
	glVertex3f(min->x, min->y, min->z);
	glVertex3f(max->x, min->y, min->z);

	glVertex3f(max->x, max->y, max->z);
	glVertex3f(min->x, max->y, max->z);
	glVertex3f(min->x, min->y, max->z);
	glVertex3f(max->x, min->y, max->z);

	glVertex3f(max->x, min->y, min->z);
	glVertex3f(min->x, min->y, min->z);
	glVertex3f(min->x, max->y, min->z);
	glVertex3f(max->x, max->y, min->z);

	glVertex3f(min->x, max->y, max->z);
	glVertex3f(min->x, max->y, min->z);
	glVertex3f(min->x, min->y, min->z);
	glVertex3f(min->x, min->y, max->z);

	glVertex3f(max->x, max->y, min->z);
	glVertex3f(max->x, max->y, max->z);
	glVertex3f(max->x, min->y, max->z);
	glVertex3f(max->x, min->y, min->z);
	glEnd();

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
}

void BoxCollider::RegenerateCollider(Vector3* pos, Vector3* size){
	delete min;
	delete max;

	max = new Vector3(pos->x + (size->x) / 2, pos->y + (size->y) / 2, pos->z + (size->z) / 2);
	min = new Vector3(pos->x - (size->x) / 2, pos->y - (size->y) / 2, pos->z - (size->z) / 2);
}

bool BoxCollider::CheckCollision(GameObject* obj){
	Collider* col = obj->GetCollider();

	if(col == nullptr) return false;

	bool xCol = min->x <= col->max->x && max->x >= col->min->x;
	bool yCol = min->y <= col->max->y && max->y >= col->min->y;
	bool zCol = min->z <= col->max->z && max->z >= col->min->z;

	return xCol && yCol && zCol;
}

bool BoxCollider::CheckCollision(Collider* col){

	if(col == nullptr) return false;

	bool xCol = min->x <= col->max->x && max->x >= col->min->x;
	bool yCol = min->y <= col->max->y && max->y >= col->min->y;
	bool zCol = min->z <= col->max->z && max->z >= col->min->z;

	return xCol && yCol && zCol;
}

