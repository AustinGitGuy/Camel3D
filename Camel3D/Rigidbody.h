#pragma once

class Vector3;

class Rigidbody {

public:
	void AddForce(Vector3* direction);
	void SetVel(Vector3* newVel);
	void SetAccel(Vector3* newAccel);

	Vector3* GetVel();
	Vector3* GetAccel();

private:
	Vector3* vel;
	Vector3* accel;
	bool gravity;
	float mass;

};