#include "Camera.h"
#include "Vector3.h"
#include <math.h>
#include <GL/glew.h>
#include <algorithm>

Camera::Camera(Vector3 newPos, Vector3 newDir){
	pos = new Vector3(newPos.x, newPos.y, newPos.z);
	rot = new Vector3(newDir.x, newDir.y, newDir.z);
	rotate = new Vector3(0, 0, 0);
	angleX = asin(rot->x);
	angleY = asin(rot->y);
}

Camera::~Camera(){
	delete pos;
	delete rot;
	delete rotate;
}

void Camera::ResetCamera(){
	if(doRotate){
		float eyeX = rotate->x + rotateRad * cos(phi) * sin(theta);
		float eyeZ = rotate->z + rotateRad * cos(theta);
		gluLookAt(eyeX, pos->y, eyeZ, rotate->x, rotate->y, rotate->z, 0, 1, 0);
		theta += .01f;
	}
	else {
		gluLookAt(pos->x, pos->y, pos->z, pos->x + rot->x, pos->y + rot->y, pos->z + rot->z, 0, 1, 0);
	}
}

void Camera::Move(int x, int y, int width, int height){
	float newX = cameraX / float(width);
	float newY = cameraY / float(height);
	newX = (newX - .5f) / 2;
	newY = (newY - .5f) / 2;

	if(cameraX != -1){
		angleX += .05f * newX;
		rot->x = sin(angleX);
		rot->z = -cos(angleX);

		angleY -= .05f * newY;
		rot->y = sin(angleY);
	}
}

void Camera::SetRotateObject(Vector3 newRotate){
	delete rotate;
	rotate = new Vector3(newRotate.x, newRotate.y, newRotate.z);
	rotateRad = std::max(pos->x - newRotate.x, pos->z - newRotate.z);
	theta = 0;
}