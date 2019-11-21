#pragma once

#include "Vector3.h"

enum IntegrationType {EulerExplicit, Kinematic};

class Rigidbody {

public:
	Rigidbody(float newMass, Vector3 newPos, IntegrationType newPosType);
	void AddForce(Vector3 newForce);
	void SetVel(Vector3 newVel);
	void SetAccel(Vector3 newAccel);
	void UpdateAccel();
	void UpdatePosAndRot(float time);

	Vector3 GetVel();
	Vector3 GetAccel();

	void UpdatePositionEulerExplicit(float time);
	void UpdatePositionKinematic(float time);

	friend class GameObject;

private:
	Vector3 vel;
	Vector3 accel;
	Vector3 force;
	Vector3 pos;

	bool gravity;
	float mass;
	float invMass;

	IntegrationType posType;
};