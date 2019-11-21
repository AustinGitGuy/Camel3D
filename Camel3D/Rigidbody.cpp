#include "Rigidbody.h"
#include "GameObject.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glut.h"

void Rigidbody::AddForce(Vector3 newForce){
	force += newForce;
}

void Rigidbody::SetVel(Vector3 newVel){
	vel = newVel;
}

void Rigidbody::SetAccel(Vector3 newAccel){
	accel = newAccel;
}

Vector3 Rigidbody::GetVel(){
	return vel;
}

Vector3 Rigidbody::GetAccel(){
	return accel;
}

void Rigidbody::UpdateAccel(){
	//Newton 2:
	accel = force * invMass;
	force = VECTOR3_ZERO;
}

Rigidbody::Rigidbody(float newMass, Vector3 newPos, IntegrationType newPosType){
	pos = newPos;
	mass = newMass;
	invMass = 1 / mass;
	posType = newPosType;
	force = VECTOR3_ZERO;
	vel = VECTOR3_ZERO;
	accel = VECTOR3_ZERO;
}

void Rigidbody::UpdatePosAndRot(float time){
	if(posType == IntegrationType::EulerExplicit){
		UpdatePositionEulerExplicit(time);
	}
	else if (posType == IntegrationType::Kinematic){
		UpdatePositionKinematic(time);
	}
}

void Rigidbody::UpdatePositionEulerExplicit(float time){
	pos += vel * time;
	vel += accel * time;
}

void Rigidbody::UpdatePositionKinematic(float time){
	pos += vel * time + accel * time * time * .5f;
	vel += accel * time;
}