#include "Rigidbody.h"
#include "GameObject.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glut.h"
#include "Vector3.h"

void Rigidbody::AddForce(Vector3* direction){
	//Check if this is correct
	*accel += *direction / mass;
}

void Rigidbody::SetVel(Vector3* newVel){
	vel = newVel;
}

void Rigidbody::SetAccel(Vector3* newAccel){
	accel = newAccel;
}

Vector3* Rigidbody::GetVel(){
	return vel;
}

Vector3* Rigidbody::GetAccel(){
	return accel;
}
